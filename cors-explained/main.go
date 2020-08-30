package main

import (
	"flag"
	"fmt"
	"io/ioutil"
	"log"
	"net/http"
	"net/url"
)

const (
	WhiteHost = "deadbeef.me"
)

func main() {

	port := flag.Int("port", 8080, "listening port")

	http.HandleFunc("/simple", func(out http.ResponseWriter, req *http.Request) {
		handleSimpleCORS(out, req)
		_, _ = out.Write([]byte("Simple cors response"))
	})

	http.HandleFunc("/preflight", func(out http.ResponseWriter, req *http.Request) {
		log.Printf("Method=%s", req.Method)
		if handlePreflightCORS(out, req) {
			return
		}

		handleSimpleCORS(out, req) // <-- note here!
		data, _ := ioutil.ReadAll(req.Body)
		_ = req.Body.Close()
		_, _ = out.Write([]byte("Preflight cores response: " + string(data)))
	})

	addr := fmt.Sprintf(":%d", *port)
	log.Fatal(http.ListenAndServe(addr, nil))
}

func handleSimpleCORS(out http.ResponseWriter, req *http.Request) {
	originHost := req.Header.Get("Origin")
	if originHost == "" {
		return
	}

	host, err := url.ParseRequestURI(originHost)
	if err != nil {
		log.Printf("Failed to parse request uri; uri=%s err=%+v\n", originHost, err)
		return
	}

	if host.Hostname() != WhiteHost {
		return
	}

	out.Header().Set("Access-Control-Allow-Origin", originHost)
	out.Header().Set("Access-Control-Allow-Credentials", "true")
}

func handlePreflightCORS(out http.ResponseWriter, req *http.Request) bool {
	if req.Method != http.MethodOptions {
		return false
	}

	originHost := req.Header.Get("Origin")
	host, err := url.ParseRequestURI(originHost)
	if err != nil {
		return false
	}

	if host.Hostname() != WhiteHost {
		return false
	}

	out.Header().Set("Access-Control-Allow-Origin", originHost)
	out.Header().Set("Access-Control-Allow-Methods", "GET, POST, PUT")
	out.Header().Set("Access-Control-Allow-Headers", "Content-Type")

	return true
}
