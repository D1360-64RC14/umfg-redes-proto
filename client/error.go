package client

import "errors"

var (
	ErrInvalidResponseFormat = errors.New("wrong response format")
	ErrInvalidResponseStatus = errors.New("invalid response status")
)
