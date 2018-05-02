package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
)

func countLines(file *os.File, counts map[string]int) {
	data := bufio.NewScanner(file)
	for data.Scan() {
		lineTokens := strings.Split(data.Text(), " ")
		for _, token := range lineTokens {
			counts[token]++
		}
	}
}

func main() {
	files := os.Args[1:]
	if len(files) == 0 {
		fmt.Println("Error: no files specified!")
		return
	}

	counts := make(map[string]int)

	for _, file := range files {
		f, err := os.Open(file)
		if err != nil {
			fmt.Printf("Error: cannot open file %v\n", err)
			continue
		}

		countLines(f, counts)

		f.Close()
	}

	fmt.Printf("-- Result --\n")

	for line, n := range counts {
		if n > 1 {
			fmt.Printf("%s -> %d\n", line, n)
		}
	}
}
