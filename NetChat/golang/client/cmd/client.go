package main

import (
	"bufio"
	"fmt"
	"io"
	"net"
	"os"
	"strings"
	"sync"
	"sync/atomic"
	"time"

	"../../codec"
)

const (
	_MaxPendingPackets = 16
)

type ChatClient struct {
	addr  string
	codec *codec.Codec

	wg     sync.WaitGroup
	closed int32
	broken int32

	outgoing     chan *codec.Packet
	outgoingStop chan struct{}

	receivingStop chan struct{}
}

func NewChatClient(addr string) *ChatClient {
	return &ChatClient{
		addr:          addr,
		closed:        0,
		broken:        0,
		outgoing:      make(chan *codec.Packet, _MaxPendingPackets),
		outgoingStop:  make(chan struct{}),
		receivingStop: make(chan struct{}),
	}
}

func (c *ChatClient) Run() {
	conn, err := net.Dial("tcp", c.addr)
	if err != nil {
		fmt.Printf("Failed to connect to %s due to %v", c.addr, err)
		return
	}

	fmt.Println("connected!")

	c.codec = codec.NewCodec(conn, conn)

	c.wg.Add(2)

	go c.send()
	go c.recv()

	cin := bufio.NewReader(os.Stdin)

	for {
		input, err := cin.ReadString('\n')
		if err != nil || atomic.LoadInt32(&c.broken) == 1 {
			break
		}

		input = strings.TrimSpace(input)

		packet := &codec.Packet{
			Timestamp: time.Now(),
		}

		var cmd string
		if codec.MatchCommand(input, &cmd) {
			packet.Type = codec.CommandPacket
			packet.Data = cmd
		} else {
			packet.Type = codec.MessagePacket
			packet.Data = input
		}

		c.outgoing <- packet
	}

	atomic.StoreInt32(&c.closed, 1)
	_ = conn.Close()

	close(c.outgoingStop)
	close(c.receivingStop)

	c.wg.Wait()
}

func (c *ChatClient) send() {
Loop:
	for {
		select {
		case packet := <-c.outgoing:
			err := c.codec.Encode(packet)
			if err != nil {
				if atomic.LoadInt32(&c.closed) != 1 {
					// May not safe.
					fmt.Printf("Failed to send packet due to %v", err)
					atomic.StoreInt32(&c.broken, 1)
				}
				break Loop
			}
		case <-c.outgoingStop:
			break Loop
		}
	}

	c.wg.Done()

	fmt.Println("Quit from sending goroutine")
}

func (c *ChatClient) recv() {
Loop:
	for {
		select {
		case <-c.receivingStop:
			break Loop
		default:
		}

		packet, err := c.codec.Decode()
		if err != nil {
			if err != io.EOF && atomic.LoadInt32(&c.closed) != 1 {
				fmt.Printf("Failed to recv packet due to %v\n", err)
				atomic.StoreInt32(&c.broken, 1)
			}
			break Loop
		}

		fmt.Println(packet.Data)
	}

	c.wg.Done()

	fmt.Println("Quit from receiving goroutine")
}
