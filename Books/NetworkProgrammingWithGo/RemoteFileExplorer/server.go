package main

import (
	"io/ioutil"
	"log"
	"net"
	"os"
	"path/filepath"
	"strings"
)

const (
	addr  = "127.0.0.1:8888"
	CD    = "CD "
	DIR   = "DIR"
	PWD   = "PWD"
	QUIT  = "QUIT"
	ERROR = "ERROR\n"
)

func main() {
	listener, err := net.Listen("tcp", addr)
	if err != nil {
		log.Fatalf("FATAL: %v", err)
	}

	log.Printf("Server is running on %s", addr)

	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Printf("WARNING: Cannot accept incoming client %v", err)
			continue
		}

		log.Printf("Connected to %s", conn.RemoteAddr())

		go handleClient(conn)
	}
}

func handleClient(conn net.Conn) {
	defer conn.Close()

	buf := make([]byte, 256)
	curDir, _ := os.Getwd()

	for {
		// Always assume we can read an entire command out of the socket.
		n, err := conn.Read(buf)
		if err != nil {
			log.Printf("WARNING: Read from client failed %v", err)
			return
		}

		req := string(buf[0:n])

		if strings.HasPrefix(req, CD) {
			onCD(conn, &curDir, strings.Trim(string(buf[len(CD):n]), "\n\r "))
		} else if strings.HasPrefix(req, DIR) {
			onDIR(conn, curDir)
		} else if strings.HasPrefix(req, PWD) {
			onPWD(conn, curDir)
		} else if strings.HasPrefix(req, QUIT) {
			conn.Write([]byte("Bye-byte"))
			return
		} else {
			conn.Write([]byte("Unknown command\n"))
		}
	}
}

func onCD(conn net.Conn, dir *string, target string) {
	if filepath.IsAbs(target) {
		*dir = target
	} else {
		newDir, err := filepath.Abs(filepath.Join(*dir, target))
		if err != nil {
			log.Printf("WARNING: Cannot absolute on %s + %s %v", *dir, target, err)
			return
		}

		*dir = newDir
	}

	conn.Write([]byte("-> " + *dir + "\n"))
}

func onDIR(conn net.Conn, dir string) {
	entries, err := ioutil.ReadDir(dir)
	if err != nil {
		log.Printf("WARNING: Cannot read entries under %s %v", dir, err)
		return
	}

	for _, file := range entries {
		conn.Write([]byte(file.Name() + "\n"))
	}
}

func onPWD(conn net.Conn, dir string) {
	conn.Write([]byte(dir + "\n"))
}
