package main

import "sync"

// Make a move if no associated entry in cache was found.
type Func func(key string) (interface{}, error)

// Cached entry.
type result struct {
	value interface{}
	err error
}

type entry struct {
	res result
	ready chan struct{}
}

type Memo struct {
	f Func
	mutex sync.Mutex
	cache map[string]*entry
}

func (m *Memo) Get(key string) (interface{}, error) {
	m.mutex.Lock()
	e, ok := m.cache[key]
	if !ok {
		e = &entry{ready: make(chan struct{})}
		m.cache[key] = e
		m.mutex.Unlock()

		e.res.value, e.res.err = m.f(key)

		close(e.ready)
	} else {
		m.mutex.Unlock()
		// If ready channel is already closed, then this expression returns immediately.
		<-e.ready
	}

	return e.res.value, e.res.err
}

func NewMemo(f Func) *Memo {
	return &Memo{f: f, cache: make(map[string]*entry)}
}