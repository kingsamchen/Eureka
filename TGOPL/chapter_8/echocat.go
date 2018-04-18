package main

import (
	"io"
	"log"
	"net"
	"os"
)

const addr = "localhost:8000"

func ioCopy(src io.Reader, dest io.Writer) {
	if _, err := io.Copy(dest, src); err != nil {
		log.Print("io failure: %v", err)
		return
	}
}

func main() {
	conn, err := net.Dial("tcp", addr)
	if err != nil {
		log.Fatal("Failed to connect to server: %v", err)
	}

	defer conn.Close()

	go ioCopy(conn, os.Stdout)
	ioCopy(os.Stdin, conn)
}
