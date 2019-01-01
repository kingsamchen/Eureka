package main

import (
	"log"
	"net"

	"../../codec"
)

const (
	_MaxPendingOutPacket = 16
)

type Connection struct {
	conn       net.Conn
	codec      *codec.Codec
	outPackets chan *codec.Packet
	stop       chan struct{}
}

func NewConnection(conn net.Conn) *Connection {
	return &Connection{
		conn:       conn,
		codec:      codec.NewCodec(conn, conn),
		outPackets: make(chan *codec.Packet, _MaxPendingOutPacket),
		stop:       make(chan struct{}),
	}
}

func (c *Connection) Init() {
	go c.writeOutPackets()
}

// Recv receives and decode a packet from underlying transmission.
func (c *Connection) Recv() (packet *codec.Packet, err error) {
	return c.codec.Decode()
}

// Send writes given data packet into internal buffer which would then be transmitted.
// This function is goroutine-safe.
func (c *Connection) Send(packet *codec.Packet) {
	c.outPackets <- packet
}

func (c *Connection) writeOutPackets() {
Loop:
	for {
		select {
		case packet := <-c.outPackets:
			if err := c.codec.Encode(packet); err != nil {
				log.Printf("Failed to write command reply to %s due to %v; pending packets will be discarded",
					c.conn.RemoteAddr(), err)
				break Loop
			}
		case <-c.stop:
			break Loop
		}
	}

	log.Printf("quit from writer goroutine of client %s", c.RemoteAddr())
}

func (c *Connection) Close() {
	close(c.stop)
	_ = c.conn.Close()
}

func (c *Connection) RemoteAddr() net.Addr {
	return c.conn.RemoteAddr()
}
