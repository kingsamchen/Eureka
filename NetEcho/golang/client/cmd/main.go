package main

import (
	"bufio"
	"flag"
	"fmt"
	"net"
	"os"
	"strings"
)

func main() {
	flag.Parse()
	ip := flag.Arg(0)
	port := flag.Arg(1)
	if ip == "" || port == "" {
		fmt.Println("Usage: client ip port")
		return
	}

	addr := ip + ":" + port

	fmt.Printf("Connecting to %s ...", addr)
	conn, err := net.Dial("tcp", addr)
	if err != nil {
		fmt.Printf("Failed to connect due to: %v\n", err)
		return
	}

	fmt.Printf("Connected\n")

	defer conn.Close()

	cin := bufio.NewReader(os.Stdin)
	buf := make([]byte, 64)

	for {
		msg, err := cin.ReadString('\n')
		if err != nil {
			fmt.Println("Bye-bye")
			return
		}

		msg = strings.TrimRight(msg, "\n")

		if _, err = conn.Write([]byte(msg)); err != nil {
			fmt.Printf("Failed to write to %s due to: %v\n", addr, err)
			break
		}

		n, err := conn.Read(buf)
		if err != nil {
			fmt.Printf("Failed to write to %s due to: %v\n", addr, err)
			break
		}

		fmt.Println(string(buf[:n]))
	}
}
