package main

import (
	"fmt"
	"time"
)

func useDatetime() {
	// of type Time
	now := time.Now()
	fmt.Println(now)
	fmt.Printf("%d-%d-%d %02d:%02d:%02d\n",
		now.Year(), now.Month(), now.Day(), now.Hour(), now.Minute(), now.Second())
}

func useTimestamp() {
	now := time.Now()

	// to unix epoch timestamp
	fmt.Println(now.Unix())
	fmt.Println(now.UnixNano())

	// convert back
	sec := now.Unix()
	rest := now.UnixNano() - sec*1000000000
	fmt.Printf("%d\n%d\n", sec, rest)
	rnow := time.Unix(sec, rest)
	fmt.Println(now.Equal(rnow))
}

func dispalyDateTime() {
	now := time.Now()
	// first part: 2006 01 02 15:04:05 is enforced by the golang standard
	// millisecond part always in the form '.000'
	ts := now.Format("2006-01-02 15:04:05.000000")
	fmt.Println(ts)
}

func relativeOrder() {
	t1 := time.Now()
	t2 := t1
	fmt.Println(t1.Equal(t2))

	t2 = t2.Add(time.Second * 10)
	fmt.Println(t1.Before(t2))
}

func main() {
	useDatetime()
	useTimestamp()
	dispalyDateTime()
	relativeOrder()
}
