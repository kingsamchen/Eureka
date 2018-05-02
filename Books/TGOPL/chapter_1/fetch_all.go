package main

import (
	"fmt"
	"io"
	"io/ioutil"
	"net/http"
	"os"
	"time"
)

func fetch(url string, ch chan<- string) {
	start := time.Now()
	response, err := http.Get(url)
	if err != nil {
		ch <- fmt.Sprint(err)
		return
	}

	bytesWritten, err := io.Copy(ioutil.Discard, response.Body)
	response.Body.Close()

	if err != nil {
		ch <- fmt.Sprintf("error while reading %s: %v", url, err)
		return
	}

	dur := time.Since(start).Seconds()
	ch <- fmt.Sprintf("%.2fs %7d %s", dur, bytesWritten, url)
}

func main() {
	start := time.Now()

	ch := make(chan string)

	for _, url := range os.Args[1:] {
		go fetch(url, ch)
	}

	for range os.Args[1:] {
		fmt.Println(<-ch)
	}

	fmt.Printf("%.2f seconds elapsed\n", time.Since(start).Seconds())
}
