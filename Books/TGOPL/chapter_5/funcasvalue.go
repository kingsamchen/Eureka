package main

import (
    "fmt"
)

func add(x, y int) int {
    return x + y
}

func mul(x, y int) int {
    return x * y
}

func calc(x, y int, f func(int, int) int) int {
    return f(x, y)
}

func main() {
    fmt.Println(calc(5, 10, add))
    fmt.Println(calc(5, 10, mul))
}
