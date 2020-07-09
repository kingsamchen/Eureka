package main

import (
	"encoding/json"
	"fmt"
)

type person struct {
	Name   string `json:"name"`
	Gender int    `json:"gender"`
}

type aggregate struct {
	Tag    string `json:"tag"`
	Ids    []int  `json:"ids"`
	Person person `json:"person"`
}

func main() {
	a := &aggregate{
		Tag: "test",
		Ids: []int{1, 3, 5},
		Person: person{
			Name:   "test",
			Gender: 1,
		},
	}

	data, _ := json.Marshal(a)
	fmt.Println(string(data))
}
