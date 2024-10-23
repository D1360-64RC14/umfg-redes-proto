package client

import (
	"fmt"
	"net"
	"time"

	"github.com/D1360-64RC14/umfg-redes-proto/proto"
)

const (
	ConnTimeout = 5 * time.Second
	MaxReadSize = 1024
)

type Connection struct {
	net.Conn
}

func NewConnection(address string) (*Connection, error) {
	var conn Connection
	var err error

	conn.Conn, err = net.DialTimeout("tcp", address, ConnTimeout)
	if err != nil {
		return nil, fmt.Errorf("NewConnection(): %w", err)
	}

	return &conn, nil
}

func (c *Connection) Send(message proto.Message) (proto.Response, error) {
	_, err := c.Write([]byte(encodeMessage(message)))
	if err != nil {
		return proto.Response{}, fmt.Errorf("Send(): %w", err)
	}

	data := make([]byte, MaxReadSize)

	size, err := c.Read(data)
	if err != nil {
		return proto.Response{}, fmt.Errorf("Send(): %w", err)
	}

	res, err := decodeResponse(string(data[:size]))
	if err != nil {
		return proto.Response{}, fmt.Errorf("Send(): %w", err)
	}

	return res, nil
}

func (c *Connection) AskIP() (proto.Response, error) {
	res, err := c.Send(proto.Message{
		Method:    proto.MethodASK,
		Endpoint:  "IP",
		Parameter: proto.NewParameter("", ""),
	})
	if err != nil {
		return proto.Response{}, fmt.Errorf("AskIP(): %w", err)
	}

	return res, nil
}

func (c *Connection) RegisterUser(ra, password string) (proto.Response, error) {
	res, err := c.Send(proto.Message{
		Method:    proto.MethodREG,
		Endpoint:  "USER",
		Parameter: proto.NewParameter(ra, password),
	})
	if err != nil {
		return proto.Response{}, fmt.Errorf("RegUser(): %w", err)
	}

	return res, nil
}

func (c *Connection) AskSecret(ra, password string) (proto.Response, error) {
	res, err := c.Send(proto.Message{
		Method:    proto.MethodASK,
		Endpoint:  "SECRET",
		Parameter: proto.NewParameter(ra, password),
	})
	if err != nil {
		return proto.Response{}, fmt.Errorf("AskSecret(): %w", err)
	}

	return res, nil
}

func (c *Connection) AskScore(ra, password string) (proto.Response, error) {
	res, err := c.Send(proto.Message{
		Method:    proto.MethodASK,
		Endpoint:  "SCORE",
		Parameter: proto.NewParameter(ra, password),
	})
	if err != nil {
		return proto.Response{}, fmt.Errorf("AskScore(): %w", err)
	}

	return res, nil
}
