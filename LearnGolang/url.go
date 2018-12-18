package main

import (
	"fmt"
	"net/url"
)

func urlComponents(location string) {
	uri, _ := url.Parse(location)
	fmt.Print(uri.Hostname(), "\n", uri.Port(), "\n", uri.EscapedPath())
}

func urlQueryString(location string) {
	uri, _ := url.Parse(location)
	qs := uri.Query()
	for k, v := range qs {
		fmt.Println(k, " -> ", v)
	}
}

func main() {
	urlComponents("https://test-my-url.com/test?key=blah")
	urlQueryString("http://http.bin.com/get?access_key=token123&uid=789&appkey=winant-http&appkey=backup-winant")
}
