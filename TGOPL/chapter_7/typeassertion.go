package main

import (
	"fmt"
)

type metadata interface {
	repr() string
}

type stringer interface {
	string() string
}

func verifyType(x interface{}) {
	if x, ok := x.(metadata); ok {
		fmt.Printf("metadata matched -> %s\n", x.repr())
	}

	if str, ok := x.(stringer); ok {
		fmt.Printf("Stringer matched -> %s\n", str.string())
	}
}

type Point struct {
	x, y int
}

func (pt *Point) repr() string {
	return "type Point"
}

func (pt *Point) string() string {
	return fmt.Sprintf("(%d, %d)", pt.x, pt.y)
}

func verifyPointType(m metadata) {
	if _, ok := m.(*Point); ok {
		fmt.Print("Matched!")
	} else {
		fmt.Print("Not Matched!")
	}

	fmt.Println(m.repr())
}

type metaD struct{}

func (m *metaD) repr() string {
	return "type metaD"
}

func main() {
	p := Point{1, 2}
	verifyType(&p)

	verifyPointType(&p)
	verifyPointType(&metaD{})
}
