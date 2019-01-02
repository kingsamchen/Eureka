package codec

import (
	"bufio"
	"bytes"
	"encoding/binary"
	"errors"
	"io"
	"regexp"
	"time"
)

// One packet in binary format:
// type | len | data
// -----+-----+------
//  1-b   2-b   len-b
// Encode: from packet to data stream
// Decode: from data stream to packet

const (
	HeaderSize = 3
)

const (
	PacketTypeStart = 1

	CommandPacket = 1
	MessagePacket = 2

	PacketTypeEnd
)

const (
	CmdList = "LIST"
	CmdName = "NAME"
	CmdUseName = "USE-NAME"
)

var (
	re *regexp.Regexp
)

func init() {
	var err error
	re, err = regexp.Compile("^\\s*\\$([\\w\\-]+)\\$(\\s+([\\w\\d\\-_]+))?\\s*$")
	if err != nil {
		panic(err)
	}
}

type Packet struct {
	Type      int8
	Data      string
	Timestamp time.Time
}

type Codec struct {
	decoder   *bufio.Reader
	decodeBuf []byte
	encoder   *bufio.Writer
	encodeBuf bytes.Buffer
}

func NewCodec(r io.Reader, w io.Writer) *Codec {
	return &Codec{
		decoder:   bufio.NewReader(r),
		decodeBuf: make([]byte, 16),
		encoder:   bufio.NewWriter(w),
	}
}

func MatchCommand(str string, cmd *string) bool {
	results := re.FindStringSubmatch(str)
	if len(results) == 0 {
		return false
	}

	*cmd = results[1]
	if len(results[3]) != 0 {
		*cmd += " " + results[3]
	}

	return true
}

func (c *Codec) ensureDecodeBufSize(required int) []byte {
	bufLen := len(c.decodeBuf)

	for bufLen < required {
		bufLen *= 2
	}

	if bufLen > len(c.decodeBuf) {
		c.decodeBuf = make([]byte, bufLen)
	}

	return c.decodeBuf[:required]
}

func (c *Codec) Decode() (packet *Packet, err error) {
	header, err := c.decoder.Peek(HeaderSize)
	if err != nil {
		return nil, err
	}

	if header[0] < PacketTypeStart || header[0] > PacketTypeEnd {
		return nil, errors.New("invalid packet type")
	}

	dataLen := int16(binary.BigEndian.Uint16(header[1:]))
	if dataLen <= 0 {
		return nil, errors.New("invalid data length")
	}

	if _, err := c.decoder.Discard(HeaderSize); err != nil {
		return nil, err
	}

	buf := c.ensureDecodeBufSize(int(dataLen))
	if _, err = io.ReadFull(c.decoder, buf); err != nil {
		return nil, err
	}

	packet = &Packet{
		Type: int8(header[0]),
		Data: string(buf),
		Timestamp: time.Now(),
	}

	return
}

func (c *Codec) Encode(packet *Packet) (err error) {
	defer c.encodeBuf.Reset()

	c.encodeBuf.WriteByte(byte(packet.Type))
	if err = binary.Write(&c.encodeBuf, binary.BigEndian, int16(len(packet.Data))); err != nil {
		return err
	}

	c.encodeBuf.WriteString(packet.Data)

	if _, err = c.encoder.Write(c.encodeBuf.Bytes()); err != nil {
		return err
	}

	if err = c.encoder.Flush(); err != nil {
		return err
	}

	return
}
