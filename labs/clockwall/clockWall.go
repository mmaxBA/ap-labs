package main

import (
	"io"
	"log"
	"net"
	"os"
	"fmt"
	"strings"
)

func handleConnection(port string, wait chan int){
	conn, err := net.Dial("tcp", port)
	if err != nil {
		log.Fatal(err)
	}
	done := make(chan int)
	go func() {
		io.Copy(os.Stdout, conn)
		log.Println("done")
		done <- 2
		wait <- 1
	}()

	x := 1
	x = <-done

	log.Println("Channel Closed with value: ", x)
	close(done)
}
func main() {
	if len(os.Args) < 2 {
		fmt.Println("Not enough arguments")
		return
	}
	wait := make(chan int)
	for i:=1;i<len(os.Args);i++{
		port := os.Args[i]
		port = strings.Split(port, "=")[1]
		go handleConnection(port, wait)
	}
	<-wait
	close(wait)

}