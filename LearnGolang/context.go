package main

import (
	"context"
	"fmt"
	"time"
)

type key string

func doTask(ctx context.Context, fn func()) (err error) {
	complete := make(chan struct{}, 1)

	go func() {
		fn()
		close(complete)
	}()

	select {
	case <-ctx.Done():
		return ctx.Err()
	case <-complete:
		return nil
	}
}

func useContextTimeout() {
	c, cancel := context.WithTimeout(context.Background(), time.Second*2)
	defer cancel()
	err := doTask(c, func() {
		fmt.Println("Task sleeping")
		time.Sleep(time.Second * 5)
		fmt.Println("wake up")
	})
	if err != nil {
		fmt.Printf("%+v", err)
	}
}

func testContextTimeoutInheritance() {
	fmt.Printf("now=%v\n", time.Now())

	c, cancel1 := context.WithTimeout(context.Background(), time.Second*5)
	defer cancel1()

	// If timeout of child is larger than the timeout of the parent, it makes child context
	// equivalent to parent context
	cc, cancel2 := context.WithTimeout(c, time.Second*2)
	defer cancel2()

	err := doTask(cc, func() {
		fmt.Println("Task sleeping")
		time.Sleep(time.Second * 4)
		fmt.Println("wake up")
	})

	// Because child context timeouts in 2s, so the task will be context deadline exceeded.
	if err != nil {
		deadTime, deadline := c.Deadline()
		fmt.Printf("err=%+v\nparent-context err=%+v deadline=%t deadtime=%v", err, c.Err(), deadline, deadTime)
	}
}

func useContextWithValues() {
	consumer := func(ctx context.Context) {
		fmt.Println(ctx.Value(key("name")).(string))
	}

	ctx := context.WithValue(context.Background(), key("name"), "test")
	consumer(ctx)
}

func main() {
	useContextTimeout()
	testContextTimeoutInheritance()
	useContextWithValues()
}
