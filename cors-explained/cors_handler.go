package main

import (
	"log"
	"net/http"
	"net/url"
)

const (
	WhiteHost = "deadbeef.me"
)

type handler = func(http.ResponseWriter, *http.Request)

func wrapCORSHandler(fn handler) handler {
	return func(out http.ResponseWriter, in *http.Request) {
		if handleCORS(out, in) {
			fn(out, in)
		}
	}
}

func handleCORS(out http.ResponseWriter, req *http.Request) (shouldContinue bool) {
	if req.Method == http.MethodOptions {
		log.Printf("Accessed PATH=%s Type=preflight\n", req.URL.Path)

		originHost := req.Header.Get("Origin")
		host, err := url.ParseRequestURI(originHost)
		if err != nil {
			log.Printf("Failed to parse request uri; uri=%s err=%+v\n", originHost, err)
			return false
		}

		if host.Hostname() != WhiteHost {
			return false
		}

		// Skip validating skip method

		out.Header().Set("Access-Control-Allow-Origin", originHost)
		out.Header().Set("Access-Control-Allow-Methods", "GET, POST, OPTIONS")
		out.Header().Set("Access-Control-Allow-Headers", "Content-Type")

		// The browser will do the cache, hence saving an OPTION request in a certain duration.
		//out.Header().Set("Access-Control-Max-Age", "86400")

		// a pre-flight request
		return false
	}

	if originHost := req.Header.Get("Origin"); originHost != "" {
		log.Printf("Accessed PATH=%s Type=simple\n", req.URL.Path)

		host, err := url.ParseRequestURI(originHost)
		if err != nil {
			log.Printf("Failed to parse request uri; uri=%s err=%+v\n", originHost, err)
			return false
		}

		if host.Hostname() != WhiteHost {
			return false
		}

		out.Header().Set("Access-Control-Allow-Origin", originHost)
	}

	return true
}
