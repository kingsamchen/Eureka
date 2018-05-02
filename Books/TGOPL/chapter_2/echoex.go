package main

import (
	"flag"
	"fmt"
	"strings"
)

var sep = flag.String("s", " ", "separator")

func main() {
	flag.Parse()
	fmt.Print(strings.Join(flag.Args(), *sep))
}
