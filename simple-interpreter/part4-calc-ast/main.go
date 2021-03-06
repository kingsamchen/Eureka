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
 * part4: abstract lexer, parser, and interpreter.
 *               token           ast
 *        lexer ------- parser ------- interpreter
 */

// -*- Lexer -*-

const (
	tokInteger = iota
	tokPlus
	tokMinus
	tokMul
	tokDiv
	tokLParen
	tokRParen
	tokEOF     = -1
	tokUnknown = -2
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

type lexer struct {
	text string
	pos  int
	tok  token
}

func newLexer(text string) lexer {
	e := lexer{
		text: text,
		pos:  0,
		tok: token{
			kind: tokEOF,
		},
	}
	// Initiate
	e.nextToken()
	return e
}

// returns current character or 0 if already past the end.
func (p *lexer) peek() byte {
	if p.pos >= len(p.text) {
		return 0
	}
	return p.text[p.pos]
}

func (p *lexer) advance() {
	p.pos++
}

func (p *lexer) skipSpace() {
	for isSpace(p.peek()) {
		p.advance()
	}
}

func (p *lexer) lexInteger() string {
	integer := ""
	for unicode.IsDigit(rune(p.peek())) {
		integer += string(p.peek())
		p.advance()
	}
	return integer
}

func (p *lexer) currentToken() token {
	return p.tok
}

// case 1: skip whitespaces
// case 2: multiple digit
// case 3: supports + and -
func (p *lexer) nextToken() {
	// Try to lexize each token in one loop.
	// For spaces, it will locate to the first non-space and restart lexize again.
	for p.pos < len(p.text) {
		ch := p.peek()

		if isSpace(ch) {
			p.skipSpace()
			continue
		}

		if unicode.IsDigit(rune(ch)) {
			value := p.lexInteger()
			p.tok = token{
				kind:  tokInteger,
				value: value,
			}
			return
		}

		if ch == '+' {
			p.advance()
			p.tok = token{
				kind:  tokPlus,
				value: "+",
			}
			return
		}

		if ch == '-' {
			p.advance()
			p.tok = token{
				kind:  tokMinus,
				value: "-",
			}
			return
		}

		if ch == '*' {
			p.advance()
			p.tok = token{
				kind:  tokMul,
				value: "*",
			}
			return
		}

		if ch == '/' {
			p.advance()
			p.tok = token{
				kind:  tokDiv,
				value: "/",
			}
			return
		}

		if ch == '(' {
			p.advance()
			p.tok = token{
				kind:  tokLParen,
				value: "(",
			}
			return
		}

		if ch == ')' {
			p.advance()
			p.tok = token{
				kind:  tokRParen,
				value: ")",
			}
			return
		}

		p.tok = token{
			kind:  tokUnknown,
			value: string(ch),
		}
		return
	}

	p.tok = token{
		kind: tokEOF,
	}
}

// -*- parser -*-

type visitor interface {
	visitNum(*nodeNum) (int, error)
	visitBinOp(*nodeBinOp) (int, error)
}

type visitable interface {
	doVisit(visitor) (int, error)
	dump(visitor)
}

type nodeBinOp struct {
	tok   token
	left  visitable
	right visitable
}

func newBinOpNode(t token, l, r visitable) *nodeBinOp {
	return &nodeBinOp{
		tok:   t,
		left:  l,
		right: r,
	}
}

func (n *nodeBinOp) doVisit(v visitor) (int, error) {
	return v.visitBinOp(n)
}

func (n *nodeBinOp) dump(v visitor) {
	v.visitBinOp(n)
}

type nodeNum struct {
	tok token
}

func newNumNode(t token) *nodeNum {
	return &nodeNum{tok: t}
}

func (n *nodeNum) doVisit(v visitor) (int, error) {
	return v.visitNum(n)
}

func (n *nodeNum) dump(v visitor) {
	v.visitNum(n)
}

type parser struct {
	lexer lexer
}

func newParser(text string) parser {
	return parser{
		lexer: newLexer(text),
	}
}

func (p *parser) consumeFactor() (node visitable, err error) {
	tok := p.lexer.currentToken()
	if tok.kind == tokInteger {
		node = newNumNode(tok)
		p.lexer.nextToken()
	} else if tok.kind == tokLParen {
		p.lexer.nextToken()
		// Handle sub-lexer recursively
		node, err = p.expr()
		p.lexer.nextToken()
	} else {
		return nil, errors.Errorf("expected tokInteger or tokSubExpr but got %+v", tok)
	}

	return
}

func (p *parser) consumeTerm() (node visitable, err error) {
	node, err = p.consumeFactor()
	if err != nil {
		return nil, err
	}

	for p.lexer.currentToken().kind == tokMul || p.lexer.currentToken().kind == tokDiv {
		opToken := p.lexer.currentToken()
		p.lexer.nextToken()
		factor, err := p.consumeFactor()
		if err != nil {
			return nil, err
		}

		node = newBinOpNode(opToken, node, factor)
	}

	return
}

// lexer   := term ((plus|minus) term)*
// term   := factor ((mul|div) factor)*
// factor := integer | LPAREN lexer RPAREN
func (p *parser) expr() (node visitable, err error) {
	node, err = p.consumeTerm()
	if err != nil {
		return nil, err
	}

	for p.lexer.currentToken().kind == tokPlus || p.lexer.currentToken().kind == tokMinus {
		opToken := p.lexer.currentToken()
		p.lexer.nextToken()
		term, err := p.consumeTerm()
		if err != nil {
			return nil, err
		}

		node = newBinOpNode(opToken, node, term)
	}

	return
}

func (p *parser) parse() (node visitable, err error) {
	return p.expr()
}

// -*- interpreter -*-

type evaluator struct {
}

func (e *evaluator) visitNum(node *nodeNum) (result int, err error) {
	return node.tok.asInt()
}

func (e *evaluator) visitBinOp(node *nodeBinOp) (result int, err error) {
	opKind := node.tok.kind
	if opKind != tokPlus && opKind != tokMinus && opKind != tokMul && opKind != tokDiv {
		return 0, errors.Errorf("invalid bin-op=%+v", &node.tok)
	}

	lhs, err := node.left.doVisit(e)
	if err != nil {
		return 0, errors.Wrapf(err, "visit op=%s left child", node.tok.value)
	}

	rhs, err := node.right.doVisit(e)
	if err != nil {
		return 0, errors.Wrapf(err, "visit op=%s right child", node.tok.value)
	}

	switch opKind {
	case tokPlus:
		result = lhs + rhs
	case tokMinus:
		result = lhs - rhs
	case tokMul:
		result = lhs * rhs
	case tokDiv:
		result = lhs / rhs
	}

	return
}

type interpreter struct {
	parser parser
	eval   evaluator
}

func newInterpretor(text string) interpreter {
	return interpreter{
		parser: newParser(text),
	}
}

func (i *interpreter) interprete() (int, error) {
	ast, err := i.parser.parse()
	if err != nil {
		return 0, err
	}
	var lsp lispfy
	ast.doVisit(&lsp)
	fmt.Println(lsp.log)
	return ast.doVisit(&i.eval)
}

type lispfy struct {
	log string
}

func (l *lispfy) visitBinOp(node *nodeBinOp) (_ int, _ error) {
	node.left.dump(l)
	lhs := l.log
	node.right.dump(l)
	rhs := l.log
	l.log = fmt.Sprintf("(%s %s %s)", node.tok.value, lhs, rhs)
	return
}

func (l *lispfy) visitNum(node *nodeNum) (_ int, _ error) {
	l.log = node.tok.value
	return
}

// -*- run -*-

func main() {
	scanner := bufio.NewScanner(os.Stdin)
	for {
		fmt.Print("expr> ")
		if !scanner.Scan() {
			break
		}

		vm := newInterpretor(scanner.Text())
		result, err := vm.interprete()
		if err != nil {
			log.Printf("Error: %+v", err)
			continue
		}

		fmt.Printf("result: %d\n", result)
	}
}
