package main

import (
	"fmt"

	"github.com/zeromq/goczmq"
)

func main() {

	fmt.Println("Run client")
	// Create a new REQ socket
	rep, err := goczmq.NewReq("tcp://*:5555")

	if err != nil {
		fmt.Printf("Error creating request socket: %v\n", err)
	}
	defer rep.Destroy()

	// if err != nil {
	// 	fmt.Printf("Error creating rep socket: %v\n", err)
	// }
	// defer rep.Destroy()

}
