package main

import (
	"bytes"
	"fmt"
)

// IntSet demo
type IntSet struct {
	words []uint64
}

// Has returns true if the set contains `x`
// Returns false otherwise.
func (set *IntSet) Has(x int) bool {
	word, bit := x/64, uint(x%64)
	return word < len(set.words) && set.words[word]&(1<<bit) != 0
}

// Add adds `x` into the set.
func (set *IntSet) Add(x int) {
	word, bit := x/64, uint(x%64)
	for word >= len(set.words) {
		set.words = append(set.words, 0)
	}

	set.words[word] |= 1 << bit
}

// UnionWith make `set` an union of `set` and `other`
func (set *IntSet) UnionWith(other *IntSet) {
	for i, word := range other.words {
		if i < len(set.words) {
			set.words[i] |= word
		} else {
			set.words = append(set.words, word)
		}
	}
}

// String returns a string representation of the IntSet.
func (s *IntSet) String() string {
	var buf bytes.Buffer

	buf.WriteString("{ ")

	for i, word := range s.words {
		if word == 0 {
			continue
		}

		for j := 0; j < 64; j++ {
			if (1<<uint(j))&word != 0 {
				fmt.Fprintf(&buf, "%d ", 64*i+j)
			}
		}
	}

	buf.WriteByte('}')

	return buf.String()
}

func main() {
	var x, y IntSet
	x.Add(1)
	x.Add(144)
	x.Add(9)
	fmt.Println(x.String()) // "{1 9 144}"
	y.Add(9)
	y.Add(42)
	fmt.Println(y.String())           // "{9 42}" x.UnionWith(&y)
	fmt.Println(x.String())           // "{1 9 42 144}"
	fmt.Println(x.Has(9), x.Has(123)) // "true false"
}
