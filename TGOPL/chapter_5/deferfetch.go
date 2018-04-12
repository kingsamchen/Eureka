package main

import (
	"fmt"
	"io"
	"net/http"
	"os"
	"path"
)

func fetch(url string) (string, int64, error) {
	resp, err := http.Get(url)
	if err != nil {
		return "", 0, err
	}

	defer resp.Body.Close()

	local := path.Base(resp.Request.URL.Path)
	if local == "/" {
		local = "index.html"
	}

	f, err := os.Create(local)
	if err != nil {
		return "", 0, err
	}

	n, err := io.Copy(f, resp.Body)
	if closeErr := f.Close(); err == nil {
		err = closeErr
	}

	return local, n, err
}

func main() {
	for _, url := range os.Args[1:] {
		filename, size, err := fetch(url)
		if err != nil {
			fmt.Printf("Fetch url %s error: %v\n", url, err)
			continue
		}

		fmt.Printf("Fetched content of %s as %s in size %d\n", url, filename, size)
	}
}
