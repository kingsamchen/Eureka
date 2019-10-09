package main

import (
	"bufio"
	"fmt"
	"log"
	"os"
	"strconv"
	"unicode"

	"github.com/pkg/errors"
)

/*
 * part2: supports + - * / arithmetics
 */

const (
	tokInteger = iota
	tokPlus
	tokMinus
	tokMul
	tokDiv
	tokEOF = -1
	tokUnknown
)

type token struct {
	kind  int
	value string
}

func (t *token) asInt() (int, error) {
	return strconv.Atoi(t.value)
}

func isSpace(ch byte) bool {
	return ch == ' ' || ch == '\t'
}

func expectToken(tok token, kind int) (err error) {
	if tok.kind != kind {
		err = errors.Errorf("token type mismatch; expect=%d actual=%d", kind, tok.kind)
	}
	return
}

type expr struct {
	text string
	pos  int
	tok  token
}

func newExpr(text string) expr {
	return expr{
		text: text,
		pos:  0,
		tok: token{
			kind: tokEOF,
		},
	}
}

// returns current character or 0 if already past the end.
func (e *expr) peek() byte {
	if e.pos >= len(e.text) {
		return 0
	}
	return e.text[e.pos]
}

func (e *expr) advance() {
	e.pos++
}

func (e *expr) skipSpace() {
	for isSpace(e.peek()) {
		e.advance()
	}
}

func (e *expr) lexInteger() string {
	integer := ""
	for unicode.IsDigit(rune(e.peek())) {
		integer += string(e.peek())
		e.advance()
	}
	return integer
}

func (e *expr) currentToken() token {
	return e.tok
}

// case 1: skip whitespaces
// case 2: multiple digit
// case 3: supports + and -
func (e *expr) nextToken() {
	// Try to lexize each token in one loop.
	// For spaces, it will locate to the first non-space and restart lexize again.
	for e.pos < len(e.text) {
		ch := e.peek()

		if isSpace(ch) {
			e.skipSpace()
			continue
		}

		if unicode.IsDigit(rune(ch)) {
			value := e.lexInteger()
			e.tok = token{
				kind:  tokInteger,
				value: value,
			}
			return
		}

		if ch == '+' {
			e.advance()
			e.tok = token{
				kind:  tokPlus,
				value: string(ch),
			}
			return
		}

		if ch == '-' {
			e.advance()
			e.tok = token{
				kind:  tokMinus,
				value: string(ch),
			}
			return
		}

		if ch == '*' {
			e.advance()
			e.tok = token{
				kind:  tokMul,
				value: string(ch),
			}
			return
		}

		if ch == '/' {
			e.advance()
			e.tok = token{
				kind:  tokDiv,
				value: string(ch),
			}
			return
		}

		e.tok = token{
			kind:  tokUnknown,
			value: string(ch),
		}
		return
	}

	e.tok = token{
		kind: tokEOF,
	}
}

func (e *expr) consumeTerm() (n int, err error) {
	tok := e.currentToken()
	if err = expectToken(tok, tokInteger); err != nil {
		return 0, err
	}

	e.nextToken()

	return tok.asInt()
}

func (e *expr) consumeFactor() (result int, err error) {
	result, err = e.consumeTerm()
	if err != nil {
		return 0, err
	}

	for e.currentToken().kind == tokMul || e.currentToken().kind == tokDiv {
		opToken := e.currentToken()

		e.nextToken()
		term, err := e.consumeTerm()
		if err != nil {
			return 0, err
		}

		if opToken.kind == tokMul {
			result *= term
		} else {
			result /= term
		}
	}

	return
}

func (e *expr) expr() (result int, err error) {
	// Initiate
	e.nextToken()

	result, err = e.consumeFactor()
	if err != nil {
		return 0, err
	}

	for e.currentToken().kind == tokPlus || e.currentToken().kind == tokMinus {
		opToken := e.currentToken()

		e.nextToken()
		factor, err := e.consumeFactor()
		if err != nil {
			return 0, err
		}

		if opToken.kind == tokPlus {
			result += factor
		} else {
			result -= factor
		}
	}

	return
}

func main() {
	scanner := bufio.NewScanner(os.Stdin)
	for {
		fmt.Print("calc> ")
		if !scanner.Scan() {
			break
		}

		e := newExpr(scanner.Text())
		result, err := e.expr()
		if err != nil {
			log.Printf("Error: %+v", err)
			continue
		}

		fmt.Printf("expr: %s\nresult: %d\n", e.text, result)
	}
}
