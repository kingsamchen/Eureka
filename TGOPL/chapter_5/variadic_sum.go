package main

import "fmt"

func sum(vals ...int) int {
	var total int
	for _, val := range vals {
		total += val
	}

	return total
}

func main() {
	fmt.Println(sum())
	fmt.Println(sum(1))
	fmt.Println(sum(1, 3, 5, 7))
}
