package main

import (
	"context"
	"fmt"
	"time"
)

func useTimedChannel(d time.Duration) {
	ch := time.After(d)
	// Block on the timer.
	<-ch
	fmt.Printf("use time.After(%ds)\n", d/time.Second)
}

func useTimedTask() {
	const t = time.Second * 3

	C := make(chan struct{})

	// timer.C of returned timer is useless. Don't use it.
	_ = time.AfterFunc(t, func() {
		// This function runs on another goroutine.
		fmt.Println("timer expires")
		close(C)
	})

	<-C
	fmt.Println("<-timer.C returns")
}

func useNewTimer() {
	const d = time.Second * 3

	t := time.NewTimer(d)
	<-t.C
	fmt.Println("Return from t.C")
}

func cancelTimer() {
	const (
		_Threshold = time.Second * 3
		_Interval  = time.Second * 5
	)

	ctx, _ := context.WithTimeout(context.Background(), _Threshold)
	t := time.NewTimer(_Interval)

	select {
	case <-ctx.Done():
		if !t.Stop() {
			<-t.C
			fmt.Println("The timer may have been expired")
		} else {
			fmt.Println("Stop the timer successfully")
		}
	case <-t.C:
		fmt.Println("Timer expires")
	}
}

func main() {
	useTimedChannel(time.Second * 3)
	useTimedTask()
	useNewTimer()
	cancelTimer()
}
