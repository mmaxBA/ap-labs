// Clock2 is a concurrent TCP server that periodically writes the time.
package main

import (
	"io"
	"log"
	"net"
	"time"
	"os"
	"flag"
)

	// TZ=US/Eastern    go run clock2.go -port 8010 &
	// TZ=Asia/Tokyo   go run clock2.go -port 8020 &
	// TZ=Europe/London    go run clock2.go -port 8030 &

// time.in(US/Eastern)
func handleConn(c net.Conn, timezone string) {
	defer c.Close()
	for {
		// t, err := TimeIn(time.Now(), "Asia/Shanghai")
		location, err := time.LoadLocation(timezone)
		if err == nil {
			_, err := io.WriteString(c, timezone+":"+time.Now().In(location).Format("15:04:05\n"))
			if err != nil {
				return // e.g., client disconnected
			}
			time.Sleep(1 * time.Second)
		}
	}
}

func TimeIn(t time.Time, name string) (time.Time, error) {
    loc, err := time.LoadLocation(name)
    if err == nil {
        t = t.In(loc)
    }
    return t, err
}

func main() {
	port := flag.String("port", "9090", "a string")
	flag.Parse()
	tz := os.Getenv("TZ")
	listener, err := net.Listen("tcp", "localhost:"+ *port)
	if err != nil {
		log.Fatal(err)
	}
	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Print(err) // e.g., connection aborted
			continue
		}
		go handleConn(conn, tz) // handle connections concurrently
	}
}
