// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 254.
//!+

// Chat is a server that lets clients chat with each other.
package main

import (
	"bufio"
	"fmt"
	"log"
	"net"
	"bytes"
	"time"
	"strings"
	"reflect"
	"math/rand"
	"flag"
	"os"
)

//!+broadcaster
type client chan<- string // an outgoing message channel
var users map[string]net.Conn //Map of users
var admin string
var isFirstUser bool
var (
	entering = make(chan client)
	leaving  = make(chan client)
	messages = make(chan string) // all incoming client messages
)

func getUsers() string {
	//Print connected users in the channel
	var buf bytes.Buffer
	buf.WriteString("irc-server > Users connected:\n")
	for k, _ := range users {
		buf.WriteString(k + " ")
	}
	buf.WriteString("\n~~~~~~~~~~~~~~~~~~~~~\n")
	return buf.String()
}

func detailUser(user string) string {
	//Prints the username and IP address if the user is connected
	_, ok := users[user]
	if ok {
		return ("irc-server > " + user + " IP: " + users[user].RemoteAddr().String())
	} else {
		return "User not recognized"
	}

}

func getTime() string {
	var buf bytes.Buffer
	buf.WriteString("irc-server > Local time: ")
	buf.WriteString(time.Now().Format(time.RFC850))
	return buf.String()
}

func sendMessageToUser(origin, destination string, data []string) {
	_, ok := users[destination]
	if ok {
		fmt.Fprintln(users[destination], origin+" > "+strings.Join(data, " "))
	} else {
		fmt.Fprintln(users[destination], "Invalid username")
	}
}

func removeUser(kicker, banned string) int {
	if _, ok := users[banned]; ok {
		if strings.Compare(kicker, admin)!=0{
			return 1
		} else{
			users[banned].Close()
			delete(users, banned)
			return 2
		}
	} else {
		return 3
	}
}

func SelectNewModerator(mapI interface{}) interface{} {
	keys := reflect.ValueOf(mapI).MapKeys()
	return keys[rand.Intn(len(keys))].Interface()
}

func broadcaster() {
	clients := make(map[client]bool) // all connected clients
	for {
		select {
		case msg := <-messages:
			// Broadcast incoming message to all
			// clients' outgoing message channels.
			for cli := range clients {
				cli <- msg
			}

		case cli := <-entering:
			clients[cli] = true

		case cli := <-leaving:
			delete(clients, cli)
			close(cli)
		}
	}
}

//!-broadcaster

//!+handleConn
func handleConn(conn net.Conn, user string) {
	users[user] = conn
	ch := make(chan string) // outgoing client messages
	if isFirstUser{
		admin = user
		isFirstUser = false
	}
	fmt.Println("irc-server > New connected user[", user, "]")
	go clientWriter(conn, ch)
	ch <- "irc-server > Your user " + user + " is successfully logged"
	if strings.Compare(user, admin) == 0 {
		ch <- "irc-server > " + user + " you are the administrator!"
		fmt.Println("irc-server > " + user + " is the administrator!")
	}
	messages <-  "irc-server > New connected user ["+ user +"]"
	entering <- ch

	input := bufio.NewScanner(conn)
	for input.Scan() {
		data := input.Text()
		data_len := len(data)
		if data_len <= 0 {
			messages <- user + "> " + input.Text()
			continue
		}
		if strings.Compare(string(data[0]), "/") != 0 {
			messages <- user + "> " + input.Text()
			continue
		}
		command := strings.Split(
			strings.Trim(data, " "),
			" ")
		if len(command) == 1 {
			switch command[0] {
			case "/users":
				ch <- getUsers()
			case "/time":
				ch <- getTime()
			default:
				messages <- user + "> " + data
			}
		} else if len(command) == 2 {
			if strings.Compare(command[0], "/user") == 0 {
				ch <- detailUser(command[1])
			}
			if strings.Compare(command[0], "/kick") == 0 {
				a :=  removeUser(user, command[1])
				if a == 1{
					ch <- "Only the administrator can remove users from the channel"
				}
				if a == 2  {
					ch <- "irc-server >" + command[1] + " was kicked"
					fmt.Println("irc-server >", command[1], "was kicked")
				}
				if a == 3 {
					ch <- "User doesn't exist"
				}
			}
		} else if len(command) >= 3 {
			if strings.Compare(command[0], "/msg") == 0 {
				sendMessageToUser(user, command[1], command[2:])
			}

		}

	}
	if strings.Compare(user, admin) == 0{
		admin = SelectNewModerator(users).(string)
		messages <- "irc-server > " + admin + " is now the administrator!"
	}
	delete(users, user)

	fmt.Println("irc-server >", user, "left")

	leaving <- ch
	messages <- user + " has left"

	conn.Close()
}

func clientWriter(conn net.Conn, ch <-chan string) {
	for msg := range ch {
		fmt.Fprintln(conn, msg) // NOTE: ignoring network errors
	}
}

//!-handleConn

//!+main
func main() {
	var host string
	var port string
	flag.StringVar(&host, "host", "localhost", "server address")
	flag.StringVar(&port, "port", "8000", "port to listen")
	flag.Parse()
	isFirstUser = true
	listener, err := net.Listen("tcp", host+":"+port)
	if err != nil {
		log.Fatal(err)
		os.Exit(-1);
	}
	users = make(map[string]net.Conn)
	fmt.Println(time.Now().UTC())
	fmt.Println("irc-server > Simple IRC Server started at:", host, port)
	fmt.Println("irc-server > Ready for receiving new clients")
	go broadcaster()
	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Print(err)
			continue
		}
		user, _ := bufio.NewReader(conn).ReadString('\n')
		user = user[:len(user)-1]
		//Test is user exists
		if _, exists := users[user]; exists {
			fmt.Fprintf(conn, "irc-server > Username alredy taken.\n")
			conn.Close()
		} else {
			go handleConn(conn, user)
		}
	}
}

//!-main