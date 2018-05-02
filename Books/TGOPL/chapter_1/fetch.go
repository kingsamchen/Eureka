package main

import (
	"fmt"
	"io/ioutil"
	"net/http"
	"os"
)

func main() {
	for _, url := range os.Args[1:] {
		response, err := http.Get(url)
		if err != nil {
			fmt.Printf("Cannot retrieve content from %s due to %v\n", url, err)
			return
		}

		body, err := ioutil.ReadAll(response.Body)
		response.Body.Close()
		if err != nil {
			fmt.Printf("Cannot read response: %v\n", err)
			return
		}

		fmt.Printf("Response of %s -> \n%s\n", url, body)
	}
}
