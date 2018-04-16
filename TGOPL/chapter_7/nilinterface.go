package main

import (
	"fmt"
)

type MyStringer interface {
	String() string
}

type Point struct {
	x, y int
}

func (p *Point) String() string {
	return fmt.Sprintf("(%d, %d)", p.x, p.y)
}

func main() {
	var i MyStringer
	fmt.Println(i == nil)

	i = (*Point)(nil)
	fmt.Println(i == nil)
}
