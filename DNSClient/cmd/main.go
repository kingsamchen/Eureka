package main

import (
	"bytes"
	"encoding/binary"
	"errors"
	"flag"
	"fmt"
	"log"
	"math"
	"math/rand"
	"net"
	"strings"
	"time"
)

const (
	_HeaderSize = 12

	_RecordTypeARecord = 0x01
	_RecordTypeCNAME = 0x05
)

func checkNoFail(err error) {
	if err != nil {
		panic(err)
	}
}

func newQueryHeader(packet *bytes.Buffer) (id uint16) {
	// ID
	rnd := rand.New(rand.NewSource(time.Now().UnixNano()))
	id = uint16(rnd.Intn(math.MaxUint16))
	checkNoFail(binary.Write(packet, binary.BigEndian, id))

	// Set RD bit flag only.
	packet.Write([]byte{0x01, 0x00})

	// QD Count
	checkNoFail(binary.Write(packet, binary.BigEndian, uint16(1)))

	// AN Count
	checkNoFail(binary.Write(packet, binary.BigEndian, uint16(0)))

	// NS Count
	checkNoFail(binary.Write(packet, binary.BigEndian, uint16(0)))

	// AR Count
	checkNoFail(binary.Write(packet, binary.BigEndian, uint16(0)))

	if packet.Len() != _HeaderSize {
		panic(errors.New(fmt.Sprintf("incorrect header size; size=%d", packet.Len())))
	}

	return
}

func newQuerySection(domain string, packet *bytes.Buffer) {
	sections := strings.Split(domain, ".")
	for _, section := range sections {
		lenByte := uint8(len(section))
		packet.WriteByte(byte(lenByte))
		packet.WriteString(section)
	}
	packet.WriteByte(0x00)

	// QTYPE = A record
	checkNoFail(binary.Write(packet, binary.BigEndian, uint16(0x0001)))
	// QCLASS = Internet address
	checkNoFail(binary.Write(packet, binary.BigEndian, uint16(0x0001)))
}

func newDNSQuery(domain string) (packet []byte) {
	var buf bytes.Buffer

	_ = newQueryHeader(&buf)
	newQuerySection(domain, &buf)

	packet = buf.Bytes()

	return
}

func readResponse(conn *net.UDPConn) (resp []byte, err error) {
	buf := make([]byte, 512)
	n, _, err := conn.ReadFrom(buf)
	if err != nil {
		return nil, err
	}

	resp = buf[:n]

	return
}

func resolveSectionDataAsString(data []byte) (str string) {
	var sections []string
	for i := 0; i < len(data); {
		secLen := int(data[i])
		if secLen == 0 {
			break
		}
		i++
		sections = append(sections, string(data[i:i+secLen]))
		i += secLen
	}
	str = strings.Join(sections, ".")
	return
}

func resolveSectionDataAsIPAddr(data []byte) (ip string) {
	if len(data) != 4 {
		panic("data should be 4-byte ip addr")
	}
	ip = fmt.Sprintf("%d.%d.%d.%d", data[0], data[1], data[2], data[3])
	return
}

type dnsRecord struct {
	Type int
	TTL int
	Name string
	Value string
}

type dnsReply struct {
	AuthorativeAnswer bool
	records []*dnsRecord
}

func parseResponse(resp []byte) (reply *dnsReply, err error) {
	reply = &dnsReply{}

	// Header section first.

	flag1 := resp[2]
	flag2 := resp[3]

	// QR | OPCODE | AA | TC | RD

	if (flag1 & 0x80) == 0 {
		log.Printf("flag1=%x", flag1)
		return nil, errors.New("not a answer reply")
	}

	if ((flag1 >> 2) & 0x1) != 0 {
		reply.AuthorativeAnswer = true
	}

	if ((flag1 >> 1) & 0x1) != 0 {
		log.Printf("flag1=%x", flag1)
		return nil, errors.New("response data was truncated")
	}

	// RA | Z | RCODE

	if (flag2 & 0x80) == 0 {
		log.Printf("flag2=%x", flag2)
		return nil, errors.New("remote server doesn't support recursive query")
	}

	if (flag2 & 0x0F) != 0 {
		log.Printf("flag2=%x", flag2)
		return nil, errors.New("error response")
	}

	ansCount := binary.BigEndian.Uint16(resp[6:8])

	cursor := 0
	for i := _HeaderSize; i < len(resp); i++ {
		if resp[i] == 0x00 {
			cursor = i + 1
			break
		}
	}

	// skip QTYPE & QCLASS
	cursor += 4

	// process answer sections
	for ansCount > 0 {
		if (resp[cursor] & 0xC0) != 0xC0 {
			log.Printf("Name=%x", resp[cursor])
			return nil, errors.New("answer doesn't use compression")
		}

		offset := int(binary.BigEndian.Uint16([]byte{resp[cursor] & 0x3F, resp[cursor+1]}))
		name := resolveSectionDataAsString(resp[offset:])
		cursor += 2

		recordType := int(binary.BigEndian.Uint16(resp[cursor:cursor+2]))
		cursor += 2

		// skip CLASS
		cursor += 2

		ttl := int(binary.BigEndian.Uint32(resp[cursor:cursor+4]))
		cursor += 4

		valueLen := int(binary.BigEndian.Uint16(resp[cursor:cursor+2]))
		cursor += 2

		var value string
		if recordType == _RecordTypeARecord {
			value = resolveSectionDataAsIPAddr(resp[cursor:cursor + valueLen])
		} else if recordType == _RecordTypeCNAME {
			value = resolveSectionDataAsString(resp[cursor:cursor + valueLen])
		}
		cursor += valueLen

		record := &dnsRecord{
			Type: recordType,
			TTL: ttl,
			Name: name,
			Value: value,
		}

		reply.records = append(reply.records, record)

		ansCount--
	}

	return
}

func dumpAsHexData(data []byte) string {
	hex := ""
	for _, b := range data {
		hex += fmt.Sprintf("%02x ", b)
	}
	return hex
}

func main() {
	dnsSvr := flag.String("dns-server", "192.168.2.1", "DNS server")
	flag.Parse()

	fmt.Printf("DNS Server: %s\n\n", *dnsSvr)

	domain := flag.Arg(0)
	queryPacket := newDNSQuery(domain)

	remoteAddr := &net.UDPAddr{IP: net.ParseIP(*dnsSvr), Port: 53}
	conn, err := net.DialUDP("udp", nil, remoteAddr)
	if err != nil {
		log.Printf("Failed to connect to remote dns server; error=%v addr=%s", err, remoteAddr.String())
		return
	}

	defer conn.Close()

	_, err = conn.Write(queryPacket)
	if err != nil {
		log.Printf("Failed to write packet data into conn; error=%v", err)
		return
	}

	resp, err := readResponse(conn)
	if err != nil {
		log.Printf("Failed to read response packet data from the conn; error=%v", err)
		return
	}

	log.Println(dumpAsHexData(resp))

	reply, err := parseResponse(resp)
	if err != nil {
		log.Printf("Failed to parse response; error=%v\n%s", err, dumpAsHexData(resp))
		return
	}

	fmt.Printf("Authorititive-answer: %t\n\n", reply.AuthorativeAnswer)

	for _, rec := range reply.records {
		var typeDesc, valueKind string
		if rec.Type == _RecordTypeARecord {
			typeDesc = "A Record"
			valueKind = "Address"
		} else if rec.Type == _RecordTypeCNAME {
			typeDesc = "CNAME"
			valueKind = "Domain"
		} else {
			panic("unknown record type")
		}

		fmt.Printf("Name: %s\nType: %s\nTTL: %d\n%s: %s\n\n", rec.Name, typeDesc, rec.TTL, valueKind, rec.Value)
	}
}
