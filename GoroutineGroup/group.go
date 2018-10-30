package main

import (
	"errors"
	"fmt"
	"time"
)

type actor struct {
	executor    func() error
	interruptor func(error)
}

// Group contains a group of actors to run.
type Group struct {
	actors []actor
}

// Add an actor into the group.
func (g *Group) Add(e func() error, i func(error)) {
	g.actors = append(g.actors, actor{e, i})
}

// Run the actor group.
func (g *Group) Run() error {
	results := make(chan error, len(g.actors))

	for _, a := range g.actors {
		go func(a actor) {
			results <- a.executor()
		}(a)
	}

	rv := <-results

	for _, a := range g.actors {
		a.interruptor(rv)
	}

	for i := 1; i < len(g.actors); i++ {
		<-results
	}

	return rv
}

func exampleAddBasic() {
	var g Group
	{
		cancel := make(chan struct{})
		g.Add(func() error {
			select {
			case <-time.After(time.Second):
				fmt.Printf("The first actor had its time elapsed\n")
				return nil
			case <-cancel:
				fmt.Printf("The first actor was canceled\n")
				return nil
			}
		}, func(err error) {
			fmt.Printf("The first actor was interrupted with: %v\n", err)
			close(cancel)
		})
	}
	{
		g.Add(func() error {
			fmt.Printf("The second actor is returning immediately\n")
			return errors.New("immediate teardown")
		}, func(err error) {
			// Note that this interrupt function is called, even though the
			// corresponding execute function has already returned.
			fmt.Printf("The second actor was interrupted with: %v\n", err)
		})
	}
	fmt.Printf("The group was terminated with: %v\n", g.Run())
	// Output:
	// The second actor is returning immediately
	// The first actor was interrupted with: immediate teardown
	// The second actor was interrupted with: immediate teardown
	// The first actor was canceled
	// The group was terminated with: immediate teardown
}

func main() {
	exampleAddBasic()
}
