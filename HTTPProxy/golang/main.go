/*
 @ 0xCCCCCCCC
*/

package main

import (
	"flag"
)

func main() {
	port := flag.Int("port", 8080, "listening port number")
	flag.Parse()

	server := NewServer(*port)
	server.Start()
}
