package main

import (
	"bufio"
	"fmt"
	"log"
	"os"
	"strconv"
	"unicode"
)

/*
 * part1: supports operation sequence
 *        still, an operator can be {+, -}
 */

const (
	tokInteger = iota
	tokPlus
	tokMinus
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

type expr struct {
	text string
	pos  int
}

func isSpace(ch byte) bool {
	return ch == ' ' || ch == '\t'
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

func (e *expr) tokenizeInteger() string {
	integer := ""
	for unicode.IsDigit(rune(e.peek())) {
		integer += string(e.peek())
		e.advance()
	}
	return integer
}

// case 1: skip whitespaces
// case 2: multiple digit
// case 3: supports + and -
func (e *expr) nextToken() (tok token) {
	// Try to lexize each token in one loop.
	// For spaces, it will locate to the first non-space and restart lexize again.
	for e.pos < len(e.text) {
		ch := e.peek()

		if isSpace(ch) {
			e.skipSpace()
			continue
		}

		if unicode.IsDigit(rune(ch)) {
			value := e.tokenizeInteger()
			return token{
				kind:  tokInteger,
				value: value,
			}
		}

		if ch == '+' {
			e.advance()
			return token{
				kind:  tokPlus,
				value: string(ch),
			}
		}

		if ch == '-' {
			e.advance()
			return token{
				kind:  tokMinus,
				value: string(ch),
			}
		}

		return token{
			kind:  tokUnknown,
			value: string(ch),
		}
	}

	return token{
		kind: tokEOF,
	}
}

func expectToken(tok token, kind int) (token, error) {
	if tok.kind == kind {
		return tok, nil
	}

	return token{}, fmt.Errorf("token type mismatch; expect=%d actual=%d", kind, tok.kind)
}

func (e *expr) eval() (result int, err error) {
	t, err := expectToken(e.nextToken(), tokInteger)
	if err != nil {
		return 0, err
	}

	result, err = t.asInt()
	if err != nil {
		return 0, err
	}

	for opToken := e.nextToken(); opToken.kind != tokEOF; {
		if opToken.kind != tokPlus && opToken.kind != tokMinus {
			return 0, fmt.Errorf("unexpected op token; op=%+v", &opToken)
		}

		lhs := result

		t, err = expectToken(e.nextToken(), tokInteger)
		if err != nil {
			return 0, err
		}
		rhs, err := t.asInt()
		if err != nil {
			return 0, err
		}

		if opToken.kind == tokPlus {
			result = lhs + rhs
		} else {
			result = lhs - rhs
		}

		opToken = e.nextToken()
	}

	return
}

func main() {
	scanner := bufio.NewScanner(os.Stdin)
	for scanner.Scan() {
		e := expr{
			text: scanner.Text(),
			pos:  0,
		}

		result, err := e.eval()
		if err != nil {
			log.Printf("Error: %+v", err)
			continue
		}

		fmt.Printf("expr: %s\nresult: %d\n", e.text, result)
	}
}
