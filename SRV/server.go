package main

import (
	"./kits"
	"database/sql"
	"encoding/json"
	"flag"
	"fmt"
	_ "github.com/go-sql-driver/mysql"
	"log"
	"net"
	"os"
	"os/signal"
	"strconv"
	"syscall"
)

const dbopen = "root:596487@tcp(127.0.0.1:3306)/chatdb?charset=utf8"
const BUFSIZE = 0xff
const checkpassword = "select password,nickname from userinfo where account = ?"
const insertstmt = "insert into userinfo (account,password,nickname) values(?,?,?)"

var IPNAME = make(map[string]string)
var IPCONN = make(map[string]net.Conn)
var msgchan = make(chan kits.Message,100)
var rootchan =make(chan kits.Root,100)
func init() { //初始化数据库基础连接 并未连接数据库
	signal.Ignore(syscall.SIGPIPE)
	file, err := os.OpenFile("log.txt", os.O_RDWR|os.O_APPEND|os.O_CREATE, os.ModePerm)
	if err != nil {
		fmt.Fprintf(os.Stderr, err.Error())
	}
	log.SetOutput(file)
}
func db() *sql.DB {  //返回数据库句柄
	db, err := sql.Open("mysql", dbopen)
	if err != nil {
		log.Fatalln(err)
	}
	db.Ping()
	return db
}
func main() { //主函数 打开注册与事件循环routine

	p := flag.Int("p", 8080, "use -p int == listen port , p+1 == regist port")
	flag.Parse()
	fmt.Printf("Chat server run at localhost:%d , regist server run at port:%d\n",*p,*p + 1 )

	go registroutine(p)
	go transroutine(p)
	for	true {
		select {
		case m := <-msgchan:
			{
				for _, v := range IPCONN {
					v.Write(m.Serialize())
				}
			}
		case r := <-rootchan:
			{
			for _, v := range IPCONN {
				v.Write(r.Serialize())
			}
			}
		}
	}
}

func registroutine(port *int) {
	// 注册监听 go routine
	p := strconv.Itoa(*port  + 1 )
	fmt.Println("Regist service now wait for connect")
	ln, err := net.Listen("tcp", ":"+p)
	logerr(err)
	for {
		reg, err := ln.Accept()
		logerr(err)
		go handlereg(reg)
	}
}
func transroutine(port *int){
	p := strconv.Itoa(*port)
	ln,err := net.Listen("tcp",":" + p)
	logerr(err)
	for {
		cli,err := ln.Accept()
		logerr(err)
		go handlecli(cli)
	}
}
func handlereg(reg net.Conn) { //每个注册连接响应
	defer reg.Close()
	db := db()
	buf := make([]byte, BUFSIZE)
loop:
	ln, err := reg.Read(buf)
	if ln > 0 {
		logerr(err)
		tmp := kits.Register{}
		errjson := json.Unmarshal(buf[:ln], &tmp)
		logerr(errjson)
		if tmp.Method == "REG" {
			defer db.Close()
			dberr := db.Ping()
			logerr(dberr)
			_, err := db.Exec(insertstmt, tmp.Account, kits.Enpassword(tmp.Password), tmp.Nickname)
			logerr(err)
			reg.Write([]byte("OK"))
			return
		} else {
			reg.Write([]byte("FAIL"))
			goto loop
		}
	}
}
func handlecli(cli net.Conn) {
	 ok := false
	defer cli.Close()
	defer delete(IPCONN,cli.RemoteAddr().String())  //从全局i:p conn映射删除conn 因为它要断开了
	defer delete(IPNAME,cli.RemoteAddr().String())

	buf := make([]byte, BUFSIZE)
	type Hofm struct {
		Method string
	}
	for {
		ln, err := cli.Read(buf)
		if ln == 0 || err != nil {
			break
		}
		head := Hofm{}
		tmpbuf := buf[:ln]
		json.Unmarshal(tmpbuf, &head)
		if head.Method == "LIN" {
			if login(tmpbuf,cli) {
				ok  = true
			}
		}else if head.Method == "MSG" {
			msg(tmpbuf)
		}else if head.Method == "OLU"{
			olu(cli)
		}
	}
	if ok {
		somebodyout := kits.Root{"ROT",IPNAME[cli.RemoteAddr().String()] + "退出聊天室"}
		rootchan <- somebodyout
	}
}
func logerr(err error) {
	if err != nil {
		log.Println(err)
	}
}

func login(b []byte,c net.Conn)	bool {
	var loginfo kits.Login
	json.Unmarshal(b, &loginfo)
	db := db()
	gotoconn := db.Ping()
	logerr(gotoconn)
	cipher := kits.Enpassword(loginfo.Password)
	var savedcipher, savedname string
	row := db.QueryRow(checkpassword, loginfo.Account)
	err := row.Scan(&savedcipher, &savedname)
	logret := kits.Logret{Method: "LRT", Ret: "OK", Nickname: savedname}
	logerr := kits.Logret{Method: "LRT", Ret: "KO", Nickname: "NULL"}
	if err != nil || cipher != savedcipher {
		c.Write(logerr.Serialize())
		return false
	} else {
		c.Write(logret.Serialize())
		IPCONN[c.RemoteAddr().String()] = c
		IPNAME[c.RemoteAddr().String()] = savedname
		somebodyin := kits.Root{"ROT",savedname +"加入聊天室"}
		rootchan <- somebodyin
		return true
	}
}
func msg(b []byte){
	msg := kits.Message{}
	json.Unmarshal(b,&msg)
	msgchan <- msg
}
func olu(c net.Conn){
	onlineusers := kits.Root{}
	onlineusers.Method = "ROT"
	for _,v := range IPNAME{
		onlineusers.Content += v
		onlineusers.Content += " "
	}
	c.Write(onlineusers.Serialize())
}