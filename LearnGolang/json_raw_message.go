package main

import (
	"encoding/json"
	"flag"
	"fmt"
)

var (
	testMarshal   bool
	testUnmarshal bool
)

type Packet struct {
	Cmd  string          `json:"cmd"`
	Data json.RawMessage `json:"data"`
}

type Control struct {
	Code int    `json:"code"`
	Msg  string `json:"msg"`
}

type Message struct {
	Msg string `json:"msg"`
}

func testSerialization() {
	ctrl := &Control{
		Code: 0xDEADBEEF,
		Msg:  "heartbeat",
	}

	raw, err := json.Marshal(ctrl)
	if err != nil {
		panic(err)
	}

	cp := &Packet{
		Cmd:  "ctrl",
		Data: raw,
	}

	raw, err = json.Marshal(cp)
	if err != nil {
		panic(err)
	}

	fmt.Println(string(raw))

	raw, err = json.Marshal(&Message{
		Msg: "Echo",
	})
	if err != nil {
		panic(err)
	}

	mp := &Packet{
		Cmd:  "msg",
		Data: raw,
	}

	raw, err = json.Marshal(mp)
	if err != nil {
		panic(err)
	}

	fmt.Println(string(raw))
}

func testDeserialization() {
	var err error

	p := &Packet{}
	cpJson := `{"cmd":"ctrl","data":{"code":3735928559,"msg":"heartbeat"}}`
	err = json.Unmarshal([]byte(cpJson), p)
	if err != nil {
		panic(err)
	}
	printPacket(p)

	mpJson := `{"cmd":"msg","data":{"msg":"Echo"}}`
	err = json.Unmarshal([]byte(mpJson), p)
	if err != nil {
		panic(err)
	}
	printPacket(p)
}

func printPacket(packet *Packet) {
	switch packet.Cmd {
	case "ctrl":
		ctrl := &Control{}
		_ = json.Unmarshal(packet.Data, ctrl)
		fmt.Printf("Control packet\ncode=%d\nmsg=%s\n", ctrl.Code, ctrl.Msg)
		break
	case "msg":
		msg := &Message{}
		_ = json.Unmarshal(packet.Data, msg)
		fmt.Printf("Message packet\nmsg=%s\n", msg.Msg)
		break
	default:
		panic("unreachable")
	}
}

func main() {
	flag.BoolVar(&testMarshal, "marshal", false, "")
	flag.BoolVar(&testUnmarshal, "unmarshal", false, "")
	flag.Parse()

	if testMarshal {
		testSerialization()
	}

	if testUnmarshal {
		testDeserialization()
	}
}
