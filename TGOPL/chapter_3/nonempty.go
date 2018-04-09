package main

import "fmt"

func nonempty(seq []string) []string {
	result := seq[:0]
	for _, ele := range seq {
		if ele != "" {
			result = append(result, ele)
		}
	}

	return result
}

func main() {
	strs := [...]string{"hello", "", "", "world", "test", "", "text"}
	fmt.Printf("%v\n", nonempty(strs[:]))
}
