package main

import (
	"bufio"
	"fmt"
	"os"
)

func main() {
	// built-in `make` to create an empty map.
	counts := make(map[string]int)
	input := bufio.NewScanner(os.Stdin)

	for input.Scan() {
		counts[input.Text()]++
	}

	for line, n := range counts {
		if n > 1 {
			fmt.Printf("%d\t%s", n, line)
		}
	}
}
