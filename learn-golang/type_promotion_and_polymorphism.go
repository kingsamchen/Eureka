package main

import "fmt"

type notifier interface {
	Notify()
}

type user struct {
	Name  string
	Email string
}

func (u *user) Notify() {
	fmt.Printf("sending an email to %s(%s)\n", u.Name, u.Email)
}

type admin struct {
	// Ensure type promotion
	user
	privilege int
}

type root struct {
	user
	recvEmail bool
}

func (r *root) Notify() {
	fmt.Printf("root: %s\nreceive email: %t", r.user.Name, r.recvEmail)
}

func SendNotification(notifier notifier) {
	notifier.Notify()
}

func main() {
	u := user{
		Name:  "KC",
		Email: "kc@test.com",
	}

	admin := admin{
		user: user{
			Name:  "admin",
			Email: "admin@test.com",
		},
		privilege: 1,
	}

	root := root{
		user: user{
			Name:  "root",
			Email: "root@test.com",
		},
		recvEmail: true,
	}

	SendNotification(&u)
	SendNotification(&admin)
	SendNotification(&root)
}
