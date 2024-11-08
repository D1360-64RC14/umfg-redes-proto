package proto

type Status int64

const (
	Ok              Status = 200
	BadRequest      Status = 400
	Unauthorized    Status = 401
	UserNotfound    Status = 404
	InvalidEndpoint Status = 405
	InternalError   Status = 500
	NoImplemented   Status = 501
)
