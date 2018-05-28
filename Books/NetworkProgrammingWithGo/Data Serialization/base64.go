package main

import (
	"encoding/base64"
	"fmt"
)

func main() {
	str := base64.StdEncoding.EncodeToString([]byte{0xFF, 0xFE, 0x65, 0x97})
	fmt.Println("encoded: " + str)
}
