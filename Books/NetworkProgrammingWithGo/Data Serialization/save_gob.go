package main

import (
	"encoding/gob"
	"log"
	"os"
)

type People struct {
	Name   string
	Emails []Email
}

type Email struct {
	Kind string
	Addr string
}

func main() {
	file, err := os.Create("test.dat")
	checkError(err)
	defer file.Close()
	p := People{
		Name: "Blah",
		Emails: []Email{
			{Kind: "Office", Addr: "test@office.com"},
			{Kind: "Home", Addr: "test@home.com"},
		},
	}
	saveGob(file, p)
}

func saveGob(file *os.File, obj interface{}) {
	encoder := gob.NewEncoder(file)
	encoder.Encode(obj)
	log.Print("Data is saved as gob")
}

func checkError(err error) {
	if err != nil {
		log.Fatalf("FATAL: %v", err)
	}
}
