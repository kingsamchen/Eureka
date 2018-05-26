/*
 @ 0xCCCCCCCC
*/

package main

import (
	"fmt"
	"log"
	"net"
)

type Server struct {
	addr     string
	listener net.Listener
}

func (srv *Server) Start() {
	var err error
	srv.listener, err = net.Listen("tcp", srv.addr)
	if err != nil {
		log.Fatalf("Failed to listen at %s : %v", srv.addr, err)
	}

	log.Printf("Proxy is listening at %s", srv.addr)

	srv.runLoop()
}

func (srv *Server) runLoop() {
	for {
		conn, err := srv.listener.Accept()
		if err != nil {
			log.Printf("Failed to accept an incoming connection : %v", err)
			continue
		}

		go srv.handleConnection(conn)
	}
}

func (srv *Server) handleConnection(conn net.Conn) {
	c := newConnection(conn)
	c.serve()
}

func NewServer(port int) *Server {
	return &Server{
		addr: fmt.Sprintf("localhost:%d", port),
	}
}
