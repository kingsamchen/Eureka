package main

import (
	"encoding/json"
	"fmt"
)

type endpoint struct {
	IP       string `json:"ip"`
	Port     uint16 `json:"port,string"`
	Desc     string `json:"desc,omitempty"`
	Reserved uint   `json:"-"`
}

func (e *endpoint) ToJSON() string {
	data, err := json.Marshal(e)
	if err != nil {
		return ""
	}

	return string(data[:])
}

func newEndpoint(data string) (ep *endpoint, err error) {
	ep = &endpoint{}
	err = json.Unmarshal([]byte(data), ep)
	return
}

func main() {
	e := &endpoint{
		IP:       "127.0.0.1",
		Port:     9876,
		Reserved: 0xdeadbeef,
	}

	// Output:
	// {"ip":"127.0.0.1","port":"9876"}
	fmt.Println(e.ToJSON())

	raw := `{"ip":"127.0.0.1","port":"9876", "desc":"localhost"}`
	ep, _ := newEndpoint(raw)
	fmt.Println(ep.Desc)
}
