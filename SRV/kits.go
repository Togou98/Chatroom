package kits

import (
	"crypto/sha256"
	"encoding/hex"
	//"fmt"
	"encoding/json"
	//"net"
	"log"
)

type Login struct{
	Method, Account, Password string
}
type Logret struct{
	Method,Ret,Nickname string
}
type Message struct{
	Method, Content ,Nickname string
}
type Register struct{
	Method, Account, Password, Nickname string
}
type Root struct{
	Method, Content string
}
const REG = "REG"

func Enpassword(str string)string{
	 ciphertext := sha256.Sum256([]byte(str))
	return hex.EncodeToString(ciphertext[:])
 }
 func(l *Logret)Serialize()[]byte{
 	ret , _ := json.Marshal(l)
 	return  ret
 }
func(l *Login)Serialize()[]byte{
	ret , err := json.Marshal(l)
	if err != nil {
		log.Println(err)
	}
	return ret
}
func(r *Root)Serialize()[]byte{
	ret,err := json.Marshal(r)
	if err != nil {
		log.Println(err)
	}
	return ret
}
func(m *Message)Serialize()[]byte{
	ret , err := json.Marshal(m)
	if err != nil {
		log.Println(err)
	}
	return ret
}

func(r *Register)Serialize()[]byte{
	ret , err := json.Marshal(r)
	if err != nil {
		log.Println(err)
	}
	return ret
}
