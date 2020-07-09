package main

import (
	"fmt"
	"io"
	"log"
	"net"
	"strings"
	"sync"
	"sync/atomic"
	"time"

	"../../codec"
)

type clientData struct {
	name string
}

type ChatServer struct {
	addr       string
	listener   net.Listener
	sessions   sync.Map
	sessionSeq int64
}

// NewChatServer creates a new chat-server instance.
func NewChatServer(port uint16) *ChatServer {
	return &ChatServer{
		addr: fmt.Sprintf("0.0.0.0:%d", port),
	}
}

func (s *ChatServer) Start() {
	var err error
	if s.listener, err = net.Listen("tcp", s.addr); err != nil {
		panic(err)
	}

	defer s.listener.Close()

	for {
		conn, err := s.listener.Accept()
		if err != nil {
			log.Printf("Failed to accept an incoming request due to %v", err)
			continue
		}

		go s.handleNewConnection(NewConnection(conn))
	}
}

// Runs on a dedicated goroutine.
func (s *ChatServer) handleNewConnection(conn *Connection) {
	log.Printf("Client %s is connected", conn.RemoteAddr())

	data := &clientData{
		name: fmt.Sprintf("user-%d", atomic.AddInt64(&s.sessionSeq, 1)-1),
	}

	s.broadcast(fmt.Sprintf("%s joined the chat!", data.name))

	s.sessions.Store(conn, data)

	conn.Init()

	s.serve(conn)
}

func (s *ChatServer) serve(conn *Connection) {
	for {
		packet, err := conn.Recv()
		if err != nil {
			if err != io.EOF {
				log.Printf("Failed to decode packet from connection %s due to %v", conn.RemoteAddr(), err)
			}
			break
		}

		switch packet.Type {
		case codec.CommandPacket:
			go s.handleCommand(conn, packet)
		case codec.MessagePacket:
			go s.handleMessage(conn, packet)
		default:
			log.Printf("Failed to recognize packet type %d", packet.Type)
			break
		}
	}

	s.handleDisconnection(conn)
}

func (s *ChatServer) handleDisconnection(conn *Connection) {
	log.Printf("Client %s is disconnected", conn.RemoteAddr())

	value, _ := s.sessions.Load(conn)
	userData := value.(*clientData)

	s.sessions.Delete(conn)

	conn.Close()

	s.broadcast(fmt.Sprintf("%s left the chat!", userData.name))
}

func (s *ChatServer) broadcast(msg string) {
	log.Printf("Broadcast notification: %s", msg)

	ts := time.Now()

	notification := &codec.Packet{
		Type: codec.MessagePacket,
		Data: fmt.Sprintf("[%s] *| %s", ts.Format("15:04:05"), msg),
		Timestamp: ts,
	}

	s.sessions.Range(func(key, value interface{}) bool {
		client := key.(*Connection)
		client.Send(notification)
		return true
	})
}

func (s *ChatServer) handleMessage(conn *Connection, packet *codec.Packet) {
	log.Printf("Receive message from %s: %s", conn.RemoteAddr(), packet.Data)

	ts := packet.Timestamp.Format("15:04:05")

	cur, _ := s.sessions.Load(conn)

	msg := &codec.Packet{
		Type: codec.MessagePacket,
		Data: fmt.Sprintf("[%s] %s| %s", ts, cur.(*clientData).name, packet.Data),
		Timestamp: time.Now(),
	}

	s.sessions.Range(func(key, value interface{}) bool {
		client := key.(*Connection)
		client.Send(msg)
		return true
	})
}

func (s *ChatServer) handleCommand(conn *Connection, packet *codec.Packet) {
	log.Printf("Receive command from %s: %s", conn.RemoteAddr(), packet.Data)

	prefix := packet.Timestamp.Format("[15:04:05] *|")

	if packet.Data == codec.CmdList {
		var str strings.Builder
		str.WriteString(prefix)
		str.WriteString("Current members in chat-room:")

		s.sessions.Range(func(key, value interface{}) bool {
			userData := value.(*clientData)
			str.WriteString("\n" + userData.name)
			return true
		})

		reply := &codec.Packet{
			Type:      codec.MessagePacket,
			Data:      str.String(),
			Timestamp: time.Now(),
		}

		conn.Send(reply)

		return
	}

	if packet.Data == codec.CmdName {
		value, _ := s.sessions.Load(conn)
		userData := value.(*clientData)

		reply := &codec.Packet{
			Type:      codec.MessagePacket,
			Data:      fmt.Sprintf("%sYou current nickname is %s", prefix, userData.name),
			Timestamp: time.Now(),
		}

		conn.Send(reply)

		return
	}

	if strings.HasPrefix(packet.Data, codec.CmdUseName) {
		reply := &codec.Packet{
			Type: codec.MessagePacket,
			Timestamp: time.Now(),
		}

		comp := strings.Split(packet.Data, " ")
		if len(comp) != 2 {
			reply.Data = prefix + "Incorrect usage of $USE-NAME$!"
			conn.Send(reply)
			return
		}

		value, _ := s.sessions.Load(conn)
		userData := value.(*clientData)
		userData.name = comp[1]

		reply.Data = prefix + "Your nickname has been changed to " + comp[1]
		conn.Send(reply)

		return
	}

	errReply := &codec.Packet{
		Type: codec.MessagePacket,
		Data: fmt.Sprintf("Unrecognized command %s", packet.Data),
		Timestamp: time.Now(),
	}

	conn.Send(errReply)
}
