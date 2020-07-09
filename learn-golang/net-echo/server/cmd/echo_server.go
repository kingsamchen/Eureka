package main

import (
	"fmt"
	"io"
	"log"
	"net"
	"sync/atomic"
	"time"
)

type EchoServer struct {
	addr       string
	listener   net.Listener
	connCounts int32
	msgCount   int32
}

// Start runs the echo server.
func (s *EchoServer) Start() {
	var err error
	if s.listener, err = net.Listen("tcp", s.addr); err != nil {
		panic(err)
	}

	defer s.listener.Close()

	go s.dump()

	for {
		conn, err := s.listener.Accept()
		if err != nil {
			log.Printf("Failed to accept an incoming connection: %v", err)
			continue
		}

		log.Printf("Client at %s is connected!\n", conn.RemoteAddr())
		atomic.AddInt32(&s.connCounts, 1)
		go s.serve(conn)
	}
}

func (s *EchoServer) serve(conn net.Conn) {
	defer conn.Close()

	buf := make([]byte, 64)

	for {
		n, err := conn.Read(buf)
		if err != nil {
			if err == io.EOF {
				log.Printf("Client %s disconnected\n", conn.RemoteAddr())
			} else {
				log.Printf("Failed to read from %s due to %v\n", conn.RemoteAddr(), err)
			}
			break
		}

		_, err = conn.Write(buf[:n])

		if err != nil {
			if err == io.EOF {
				log.Printf("Client %s disconnected\n", conn.RemoteAddr())
			} else {
				log.Printf("Failed to wriite to %s due to %v\n", conn.RemoteAddr(), err)
			}
			break
		}

		atomic.AddInt32(&s.msgCount, 1)
	}

	atomic.AddInt32(&s.connCounts, -1)
}

func (s *EchoServer) dump() {
	for {
		select {
		case <-time.Tick(time.Second * 5):
			log.Printf("Active connections %d | Received messages %d\n", atomic.LoadInt32(&s.connCounts),
				atomic.LoadInt32(&s.msgCount))
		}
	}
}

// NewEchoServer creates an new instance of echo server.
func NewEchoServer(port uint16) *EchoServer {
	return &EchoServer{
		addr: fmt.Sprintf(":%d", port),
	}
}
