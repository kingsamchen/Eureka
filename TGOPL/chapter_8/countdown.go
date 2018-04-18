package main

import (
	"fmt"
	"os"
	"time"
)

func main() {
	abort := make(chan struct{})
	ticker := time.NewTicker(1 * time.Second)

	go func() {
		os.Stdin.Read(make([]byte, 1))
		abort <- struct{}{}
	}()

	for i := 10; i > 0; i-- {
		select {
		case <-ticker.C:
			fmt.Println(i)
			<-ticker.C
		case <-abort:
			fmt.Println("Aborted!")
			return
		}
	}

	ticker.Stop()
	fmt.Println("Execute the task")
}
