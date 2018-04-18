package main

import (
	"fmt"
	"io/ioutil"
	"os"
	"path/filepath"
	"time"
)

func listDirEntries(dir string) []os.FileInfo {
	entries, err := ioutil.ReadDir(dir)
	if err != nil {
		fmt.Fprintf(os.Stderr, "ReadDir() fails: %v", err)
		return nil
	}

	return entries
}

func walkDir(dir string, filesizes chan<- int64) {
	for _, entry := range listDirEntries(dir) {
		if entry.IsDir() {
			walkDir(filepath.Join(dir, entry.Name()), filesizes)
		} else {
			filesizes <- entry.Size()
		}
	}
}

func printSize(files, total int64) {
	fmt.Printf("\r%d files\t %d MB", files, total / 1e6)
}

func main() {
	dir := os.Args[1]

	sizeChan := make(chan int64)
	tick := time.Tick(time.Millisecond * 500)

	go func() {
		walkDir(dir, sizeChan)
	}()

	var fileNum, totalSize int64

	for {
		select {
		case size, ok := <-sizeChan:
			if !ok {
				break
			}

			fileNum++
			totalSize += size

		case <-tick:
			printSize(fileNum, totalSize)
		}
	}

	printSize(fileNum, totalSize)
}
