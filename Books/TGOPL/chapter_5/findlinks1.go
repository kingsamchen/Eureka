package main

import (
	"fmt"
	"os"

	"golang.org/x/net/html"
)

func visit(links []string, n *html.Node) []string {
	if n.Type == html.ElementNode && n.Data == "a" {
		for _, a := range n.Attr {
			if a.Key == "href" {
				links = append(links, a.Val)
			}
		}
	}

	for c := n.FirstChild; c != nil; c = c.NextSibling {
		links = visit(links, c)
	}

	return links
}

func main() {
	doc, err := html.Parse(os.Stdin)
	if err != nil {
		fmt.Fprintf(os.Stderr, "html-parse failed: %v\n", err)
		os.Exit(1)
	}

	links := visit(nil, doc)

	for _, link := range links {
		fmt.Println(link)
	}
}
