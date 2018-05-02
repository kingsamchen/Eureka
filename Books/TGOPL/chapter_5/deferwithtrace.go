package main

import (
	"log"
	"time"
)

func trace(msg string) func() {
	start := time.Now()
	log.Printf("enter %s", msg)
	return func() {
		log.Printf("exit %s (%s)", msg, time.Since(start))
	}
}

func bigSlowOperation() {
	// Note this usage.
	defer trace("bigSlowOperation")()
	log.Println("Inside bigSlowOperation")
	time.Sleep(5 * time.Second)
}

func main() {
	bigSlowOperation()
}
