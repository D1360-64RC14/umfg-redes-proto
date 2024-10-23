package client

import (
	"fmt"
	"strconv"
	"strings"

	"github.com/D1360-64RC14/umfg-redes-proto/proto"
)

func encodeMessage(message proto.Message) string {
	return message.String()
}

func decodeResponse(response string) (proto.Response, error) {
	parts := strings.Split(strings.TrimRight(response, "\n"), " ")

	if len(parts) != 3 {
		return proto.Response{}, fmt.Errorf("%w: invalid syntax '%s'", ErrInvalidResponseFormat, response)
	}

	if parts[0] != "RESPONSE" {
		return proto.Response{}, fmt.Errorf("%w: invalid method '%s'", ErrInvalidResponseFormat, parts[0])
	}

	status, err := decodeStatus(parts[1])
	if err != nil {
		return proto.Response{}, fmt.Errorf("decodeResponse(): %w", err)
	}

	return proto.Response{
		Status: status,
		Value:  parts[2],
	}, nil
}

func decodeStatus(status string) (proto.Status, error) {
	statusNumber, err := strconv.ParseInt(status, 10, 64)
	if err != nil {
		return 0, fmt.Errorf("%w: '%s'", ErrInvalidResponseStatus, status)
	}

	return proto.Status(statusNumber), nil
}
