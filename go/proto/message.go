package proto

type Message struct {
	Method    Method
	Endpoint  string
	Parameter Parameter
}

func (m Message) String() string {
	return string(m.Method) + " " + m.Endpoint + " " + m.Parameter.Key + " " + m.Parameter.Value
}
