package main

import (
	"fmt"
	"log"
	"net/rpc"
)

type Args struct {
	Lhs int
	Rhs int
}

type Relation struct {
	Gcd int
	Lcm int
}

func main() {
	client, err := rpc.Dial("tcp", "127.0.0.1:8888")
	if err != nil {
		log.Fatalf("%v", err)
	}

	args := Args{Lhs: 7, Rhs: 15}
	var r Relation

	err = client.Call("Arith.GetRelation", &args, &r)
	if err != nil {
		log.Fatalf("%v", err)
	}

	fmt.Printf("gcd: %d\nlcm:%d", r.Gcd, r.Lcm)
}
