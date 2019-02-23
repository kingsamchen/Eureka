package main

import (
	"bytes"
	"encoding/json"
	"flag"
	"fmt"
	"io/ioutil"
	"mime/multipart"
	"net/http"
	"net/url"
	"strconv"
)

var (
	testGet            bool
	testPostURLEncoded bool
	testPostJSON       bool
	testMultipart      bool
	testHeader         bool
)

func httpGet() {
	xurl, _ := url.Parse("https://httpbin.org/get")
	queries := url.Values{}
	queries.Add("type", "test")
	queries.Add("data", "!@#$%^&*()_-=+~` ,.<>/?;:[]{}|\\")
	xurl.RawQuery = queries.Encode()

	resp, err := http.Get(xurl.String())
	if err != nil {
		panic(err)
	}

	defer resp.Body.Close()

	data, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		panic(err)
	}

	fmt.Println(string(data))
}

func httpPostURLEncoded() {
	payload := url.Values{}
	payload.Add("type", "test")
	payload.Add("approach", "www-urlencoded")
	payload.Add("data", "!@#$%^&*()_-=+~` ,.<>/?;:[]{}|\\")
	resp, err := http.PostForm("https://httpbin.org/post?index=123", payload)
	if err != nil {
		panic(err)
	}

	defer resp.Body.Close()

	data, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		panic(err)
	}

	fmt.Println(string(data))
}

type Val struct {
	Type     string `json:"type"`
	Approach string `json:"approach"`
	Status   int    `json:"status"`
	Data     string `json:"data"`
}

func httpPostJSON() {
	val := Val{
		Type:     "test",
		Approach: "json",
		Status:   123,
		Data:     "json test",
	}

	raw, err := json.Marshal(&val)
	if err != nil {
		panic(err)
	}

	data := &bytes.Buffer{}
	data.Write(raw)

	resp, err := http.Post("https://httpbin.org/post", "application/json", data)
	if err != nil {
		panic(err)
	}

	defer resp.Body.Close()

	raw, err = ioutil.ReadAll(resp.Body)
	if err != nil {
		panic(err)
	}

	fmt.Println(string(raw))
}

func httpPostMultipartFormData() {
	data := &bytes.Buffer{}

	writer := multipart.NewWriter(data)
	_ = writer.WriteField("type", "test")
	_ = writer.WriteField("approach", "multipart/form-data")
	_ = writer.WriteField("status", strconv.Itoa(123))
	_ = writer.WriteField("data", "!@#$%^&*()_-=+~` ,.<>/?;:[]{}|\\")
	_ = writer.Close()

	resp, err := http.Post("https://httpbin.org/post", writer.FormDataContentType(), data)
	if err != nil {
		panic(err)
	}

	defer resp.Body.Close()

	r, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		panic(err)
	}

	fmt.Println(string(r))
}

func httpHeaders() {
	body := &bytes.Buffer{}
	req, err := http.NewRequest("GET", "https://httpbin.org/get", body)
	if err != nil {
		panic(err)
	}

	req.Header.Add("uuid", "0xDEADBEEF")

	resp, err := http.DefaultClient.Do(req)
	if err != nil {
		panic(err)
	}

	defer resp.Body.Close()

	r, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		panic(err)
	}

	fmt.Println(string(r))
}

func main() {
	flag.BoolVar(&testGet, "test-get", false, "")
	flag.BoolVar(&testPostURLEncoded, "test-url-encoded", false, "")
	flag.BoolVar(&testPostJSON, "test-json", false, "")
	flag.BoolVar(&testMultipart, "test-multipart", false, "")
	flag.BoolVar(&testHeader, "test-header", false, "")
	flag.Parse()

	if testGet {
		httpGet()
	}

	if testPostURLEncoded {
		httpPostURLEncoded()
	}

	if testPostJSON {
		httpPostJSON()
	}

	if testMultipart {
		httpPostMultipartFormData()
	}

	if testHeader {
		httpHeaders()
	}
}
