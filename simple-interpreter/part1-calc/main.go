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
 * part1: assume operator and operand are only one character
 *        and no whitespaces can occur.
 */

const (
	tokInteger = iota
	tokPlus
	tokEOF = -1
	tokErr
)

type token struct {
	kind  int
	value string
}

type expr struct {
	text string
	pos  int
}

func (e *expr) nextToken() (tok token) {
	if e.pos >= len(e.text) {
		return token{
			kind: tokEOF,
		}
	}

	ch := e.text[e.pos]

	if unicode.IsDigit(rune(ch)) {
		e.pos++
		return token{
			kind:  tokInteger,
			value: string(ch),
		}
	}

	if ch == '+' {
		e.pos++
		return token{
			kind:  tokPlus,
			value: string(ch),
		}
	}

	return token{
		kind: tokErr,
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

	_, err = expectToken(e.nextToken(), tokPlus)
	if err != nil {
		return 0, err
	}

	right, err := expectToken(e.nextToken(), tokInteger)
	if err != nil {
		return 0, err
	}

	l, _ := strconv.Atoi(left.value)
	r, _ := strconv.Atoi(right.value)
	result := l + r

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
