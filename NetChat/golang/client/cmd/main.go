package main

import (
	"flag"
	"fmt"
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
	client := NewChatClient(addr)

	fmt.Printf("Connecting to %s ...", addr)
	client.Run()
}
