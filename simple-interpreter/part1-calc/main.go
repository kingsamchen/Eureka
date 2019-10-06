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
 * part1: two operands and one operator
 *        an operator can be {+, -}
 *        operand can be multiple digits
 *        automatically skip redundant spaces
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

func (e *expr) skipSpace() {
	for isSpace(e.peek()) {
		e.pos++
	}
}

func (e *expr) tokenizeInteger() string {
	integer := ""
	for unicode.IsDigit(rune(e.peek())) {
		integer += string(e.peek())
		e.pos++
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
			e.pos++
			return token{
				kind:  tokPlus,
				value: string(ch),
			}
		}

		if ch == '-' {
			e.pos++
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

	return token{}, fmt.Errorf("token type doesn't match; expect=%d actual=%d", kind, tok.kind)
}

func (e *expr) eval() (int, error) {
	var err error
	left, err := expectToken(e.nextToken(), tokInteger)
	if err != nil {
		return 0, err
	}

	op := e.nextToken()
	if op.kind != tokPlus && op.kind != tokMinus {
		return 0, fmt.Errorf("unexpected op token; op=%+v", &op)
	}

	right, err := expectToken(e.nextToken(), tokInteger)
	if err != nil {
		return 0, err
	}

	l, _ := strconv.Atoi(left.value)
	r, _ := strconv.Atoi(right.value)

	var result int
	if op.kind == tokPlus {
		result = l + r
	} else {
		result = l - r
	}

	return result, nil
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
