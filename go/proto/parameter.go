package proto

type Parameter struct {
	Key   string
	Value string
}

func NewParameter(key, value string) Parameter {
	return Parameter{
		Key:   key,
		Value: value,
	}
}
