package main

import (
	"fmt"
	"reflect"
)

type P struct {
	name     string
	deceased bool
}

func main() {
	p := P{}
	// main.P	struct
	fmt.Println(reflect.TypeOf(p), "\t", reflect.TypeOf(p).Kind())
}
