/*
 @ 0xCCCCCCCC
*/

package main

import (
	"bytes"
	"errors"
	"io"
	"log"
	"net"
	"net/url"
	"strings"
)

const (
	MethodConnect = "CONNECT"
)

type connection struct {
	reqConn net.Conn
}

func (c *connection) serve() {
	defer c.reqConn.Close()

	poachedData, remoteAddr, secure, err := readRequestInfo(c.reqConn)
	if err != nil {
		log.Printf("WARNING: Unable to read request info from %s : %v", c.reqConn.LocalAddr(), err)
		return
	}

	log.Printf("Establishing connection to %s", remoteAddr)

	remoteConn, err := net.Dial("tcp", remoteAddr)
	if err != nil {
		log.Printf("WARNING: Failed to connect to host %s : %v", remoteAddr, err)
		return
	}

	defer remoteConn.Close()

	// If we are going to proxy a https request, we just simply discard `poachedData`, which contains
	// the entire connect header;
	// Otherwise, we forward `poachedData` alone with remaining header and body to the remote host.
	if secure {
		c.reqConn.Write([]byte("HTTP/1.1 200 Connection Established\r\n\r\n"))
	} else {
		_, err = remoteConn.Write(poachedData)
		if err != nil {
			log.Printf("WARNING: Failed to write request header to remote host! %v", err)
			return
		}
	}

	log.Printf("Begin to tunneling connections %s <-> %s", c.reqConn.LocalAddr(), remoteAddr)

	go io.Copy(remoteConn, c.reqConn)

	io.Copy(c.reqConn, remoteConn)

	log.Print("The tunnel is ended")
}

// Read request line from `reqConn` and parse it.
func readRequestInfo(reqConn net.Conn) (poachedData []byte, addr string, secureRequest bool, err error) {
	requestLine, poachedData, err := poachRequestLine(reqConn)
	if err != nil {
		log.Printf("WARNING: Failed to read request line from request connection : %v", err)
		return
	}

	// Ignore HTTP version here by now.
	method, uri, _, ok := parseRequestLine(requestLine)
	if !ok {
		err = errors.New("malformed request line")
		return
	}

	// Drain CONNECT request header, otherwise remaining header data that survived in poaching will cause TLS
	// handshake to fail.
	if method == MethodConnect {
		secureRequest = true
		poachedData, err = drainConnectRequestHeader(reqConn, poachedData)
		if err != nil {
			return
		}
	}

	u, err := url.Parse(uri)
	if err != nil {
		log.Printf("WARNING: Failed to parse request uri %s : %v", uri, err)
		return
	}

	if secureRequest {
		addr = u.Scheme + ":" + u.Opaque
	} else {
		addr = u.Host
		if strings.Index(addr, ":") == -1 {
			addr += ":80"
		}
	}

	return poachedData, addr, secureRequest, nil
}

func poachRequestLine(reqConn net.Conn) (reqLine string, data []byte, err error) {
	var poachedData []byte
	buf := make([]byte, 64)
	for {
		var bytesRead int
		bytesRead, err = reqConn.Read(buf)
		if err != nil {
			return
		}

		poachedData = append(poachedData, buf[:bytesRead]...)
		index := bytes.Index(poachedData, []byte("\r\n"))
		if index != -1 {
			reqLine = string(poachedData[:index])
			break
		}
	}

	return reqLine, poachedData, nil
}

func parseRequestLine(line string) (method, path, ver string, ok bool) {
	tokens := strings.Split(line, " ")
	if len(tokens) != 3 {
		return
	}
	return tokens[0], tokens[1], tokens[2], true
}

func drainConnectRequestHeader(reqConn net.Conn, data []byte) ([]byte, error) {
	const BufSize = 64
	for {
		buf := make([]byte, BufSize)
		n, err := reqConn.Read(buf)
		if err != nil {
			return nil, err
		}

		data = append(data, buf[:n]...)
		if n < BufSize {
			log.Print("Drained https connect request header")
			break
		}
	}

	return data, nil
}

func newConnection(conn net.Conn) *connection {
	return &connection{
		reqConn: conn,
	}
}
