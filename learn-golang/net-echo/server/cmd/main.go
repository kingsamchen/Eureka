package main

import (
	"flag"
	"fmt"
)

func main() {
	flag.Parse()
	if err := Init(); err != nil {
		panic(err)
	}

	srv := NewEchoServer(Conf.Port)

	fmt.Println("echo server is about running on ", srv.addr)

	srv.Start()
}
