package main

import (
	"flag"
	"log"
)

func main() {
	flag.Parse()
	if err := Init(); err != nil {
		panic(err)
	}

	srv := NewChatServer(Conf.Port)

	log.Print("Chat server is about to run on ", srv.addr)

	srv.Start()
}
