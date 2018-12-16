package main

import (
	"context"
	"fmt"
	"sync"
	"time"
)

type key string

func perform(ctx context.Context) error {
	deadline, _ := ctx.Deadline()
	fmt.Println("The deadline of perform: ", deadline)

	for {
		fmt.Println("Bazinga!")

		select {
		case <-ctx.Done():
			return ctx.Err()
		case <-time.After(time.Second):
			// just wait.
		}
	}
	return nil
}

func useContextForCancelation() {
	wg := sync.WaitGroup{}
	wg.Add(1)

	ctx, cancel := context.WithTimeout(context.Background(), time.Second*5)

	go func() {
		err := perform(ctx)
		fmt.Println(err)
		wg.Done()
	}()

	time.Sleep(time.Second * 2)
	cancel()

	wg.Wait()
}

func useContextWithValues() {
	consumer := func(ctx context.Context) {
		fmt.Println(ctx.Value(key("name")).(string))
	}

	ctx := context.WithValue(context.Background(), key("name"), "test")
	consumer(ctx)
}

func main() {
	useContextForCancelation()
	useContextWithValues()
}
