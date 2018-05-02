package main

import (
	"fmt"
	"sort"
)

var prereqs = map[string][]string{
	"algorithm": {"data structures"},
	"calculus":  {"linear algebra"},
	"compilers": {
		"data structures",
		"formal languages",
		"computer organization",
	},
}

func main() {
	for i, course := range topoSort(prereqs) {
		fmt.Printf("%d:\t%s\n", i+1, course)
	}
}

func topoSort(m map[string][]string) []string {
	var order []string
	seen := make(map[string]bool)

	var visitAll func([]string)

	visitAll = func(items []string) {
		for _, item := range items {
			if !seen[item] {
				seen[item] = true
				visitAll(m[item])
				order = append(order, item)
			}
		}
	}

	var keys []string
	for key := range m {
		keys = append(keys, key)
	}

	sort.Strings(keys)
	visitAll(keys)

	return order
}
