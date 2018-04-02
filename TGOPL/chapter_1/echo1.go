package main

import (
    "fmt"
    "os"
)

func main() {
    var s, sep string
    sep = " "
    for i := 1; i < len(os.Args); i++ {
        s += os.Args[i] + sep
    }

    fmt.Println(s)
}
