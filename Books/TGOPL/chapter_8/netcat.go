package main

import (
	"io"
	"log"
	"net"
	"os"
)

func main() {
	host := os.Args[1]
	conn, err := net.Dial("tcp", host)
	if err != nil {
		log.Fatal("Failed to connect to %s : %v", host, err)
	}

	defer conn.Close()

	if _, err := io.Copy(os.Stdout, conn); err != nil {
		log.Fatal(err)
	}
}
