package main

import (
	"encoding/gob"
	"fmt"
	"log"
	"os"
	"strings"
)

type People struct {
	Name   string
	Emails []Email
}

type Email struct {
	Kind string
	Addr string
}

func (p *People) String() string {
	var s strings.Builder
	s.WriteString(fmt.Sprintf("Name: %s\nEmails:\n", p.Name))
	for _, email := range p.Emails {
		s.WriteString(fmt.Sprintf("%s\t%s\n", email.Kind, email.Addr))
	}

	return s.String()
}

func main() {
	file, err := os.Open("test.dat")
	checkError(err)
	defer file.Close()
	var p People
	loadGob(file, &p)
	log.Print(p.String())
}

func loadGob(file *os.File, obj interface{}) {
	decoder := gob.NewDecoder(file)
	decoder.Decode(obj)
	log.Print("Decode gob data completed")
}

func checkError(err error) {
	if err != nil {
		log.Fatalf("FATAL: %v", err)
	}
}
