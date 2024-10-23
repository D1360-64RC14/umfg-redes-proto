package main

import (
	"log"
	"os"

	"github.com/D1360-64RC14/umfg-redes-proto/client"
	"github.com/D1360-64RC14/umfg-redes-proto/proto"
)

const (
	ConnHost = "0.0.0.0"
	ConnPort = "9000"

	CredRA       = "1424"
	CredPassword = "abrobrinha"
)

var (
	logInfo  = log.New(os.Stdout, "[INFO] ", log.Ldate|log.Ltime|log.Lmsgprefix)
	logError = log.New(os.Stdout, "[ERROR] ", log.Ldate|log.Ltime|log.Lmsgprefix)
)

func main() {
	logInfo.Println("iniciando...")

	conn, err := client.NewConnection(ConnHost + ":" + ConnPort)
	if err != nil {
		panic(err)
	}
	defer conn.Close()

	logInfo.Println("conexão estabelecida!")

	{
		res, err := conn.AskIP()
		if err != nil {
			logError.Println(err)
			return
		}

		logInfo.Printf("conexão funcional (meu IP: %s)\n", res.Value)
	}

	{
		res, err := conn.RegisterUser(CredRA, CredPassword)
		if err != nil {
			logError.Println(err)
			return
		}

		if res.Status != proto.Ok {
			logError.Println("status não 200:", res.Status)
			return
		}

		logInfo.Println("registrado com sucesso!")
	}

	{
		res, err := conn.AskSecret(CredRA, CredPassword)
		if err != nil {
			logError.Println(err)
			return
		}

		if res.Status != proto.Ok {
			logError.Println("status não 200:", res.Status)
			return
		}

		logInfo.Println("secret:", res.Value)
	}

	{
		res, err := conn.AskScore(CredRA, CredPassword)
		if err != nil {
			logError.Println(err)
			return
		}

		if res.Status != proto.Ok {
			logError.Println("status não 200:", res.Status)
			return
		}

		logInfo.Println("score:", res.Value)
	}

	logInfo.Println("saindo...")
}
