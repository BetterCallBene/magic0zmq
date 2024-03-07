package main

import (
	"fmt"

	"github.com/vmihailenco/msgpack/v5"
	"github.com/zeromq/goczmq"
)

type OutputGoingData struct {
	Name string
}

type InputComingData struct {
	Name string
}

func NewOutputGoingData(name string) OutputGoingData {
	return OutputGoingData{
		Name: name,
	}
}

func main() {

	fmt.Println("Run client")
	// Create a new REQ socket
	rep, err := goczmq.NewReq("tcp://172.17.0.2:5555")
	rep.SetRcvtimeo(1000)

	if err != nil {
		panic(err)
	}
	defer rep.Destroy()

	output := NewOutputGoingData("Hello World!")

	b, err := msgpack.Marshal(output)
	if err != nil {
		panic(err)
	}

	// Send a message to the server
	err = rep.SendFrame(b, 0)
	if err != nil {
		panic(err)
	}

	// Receive the reply from the server
	reply, _, err := rep.RecvFrame()
	if err != nil {
		panic(err)
	}
	fmt.Println("Received", string(reply))

	var input InputComingData
	err = msgpack.Unmarshal(reply, &input)
	if err != nil {
		panic(err)
	}

	fmt.Println("Received", input.Name)

}
