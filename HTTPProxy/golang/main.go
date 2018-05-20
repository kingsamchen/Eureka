package main

import (
	"flag"
	"fmt"
	"net"
	"log"
	"net/textproto"
	"bufio"
)

func handleConnection(conn net.Conn) {
	defer conn.Close()

	protoReader := textproto.NewReader(bufio.NewReader(conn))
	requestLine, _ := protoReader.ReadLine()
	log.Println(requestLine)
}

func main() {
	port := flag.Int("port", 8080, "listening port number")
	flag.Parse()

	addr := fmt.Sprintf("localhost:%d", *port)
	listener, err := net.Listen("tcp", addr)
	if err != nil {
		log.Fatalf("Failed to listen at %s : %v", addr, err)
	}

	log.Printf("Proxy is listening at %s", addr)

	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Printf("Failed to accept an incoming connection: %v", err)
			continue
		}

		go handleConnection(conn)
	}
}
