package main

import (
	"log"
	"net/http"
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

type Arith int

func (t *Arith) GetRelation(args *Args, result *Relation) error {
	result.Gcd = gcd(args.Lhs, args.Rhs)
	result.Lcm = lcm(args.Lhs, args.Rhs)
	return nil
}

func main() {
	rpc.Register(new(Arith))
	rpc.HandleHTTP()

	err := http.ListenAndServe("127.0.0.1:8888", nil)
	if err != nil {
		log.Fatalf("%v", err)
	}
}

func gcd(a, b int) int {
	for b != 0 {
		t := b
		b = a % b
		a = t
	}
	return a
}

func lcm(a, b int) int {
	g := gcd(a, b)
	return a * b / g
}
