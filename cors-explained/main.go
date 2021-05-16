package main

import (
	"flag"
	"fmt"
	"io/ioutil"
	"log"
	"net/http"
)

func main() {

	port := flag.Int("port", 8080, "listening port")

	http.HandleFunc("/simple", wrapCORSHandler(func(out http.ResponseWriter, in *http.Request) {
		log.Println("Accessed /simple")
		_, _ = out.Write([]byte("A response for simple request"))
	}))

	http.HandleFunc("/preflight", wrapCORSHandler(func(out http.ResponseWriter, req *http.Request) {
		log.Println("Accessed /preflight")
		data, _ := ioutil.ReadAll(req.Body)
		_ = req.Body.Close()
		_, _ = out.Write([]byte("Preflight cores response: " + string(data)))
	}))

	addr := fmt.Sprintf(":%d", *port)
	log.Printf("Listening at %s", addr)
	log.Fatal(http.ListenAndServe(addr, nil))
}
