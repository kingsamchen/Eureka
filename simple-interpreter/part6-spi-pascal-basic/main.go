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
 * part6: add support of pascal basic definitions.
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
	tokBegin
	tokEnd
	tokVar
	tokSemi
	tokDot
	tokAssign
	tokEOF     = -1
	tokUnknown = -2
)

type token struct {
	kind  int
	value string
}

var (
	_Reserved = map[string]*token{
		"BEGIN": {
			kind:  tokBegin,
			value: "BEGIN",
		},
		"END": {
			kind:  tokEnd,
			value: "END",
		},
	}
)

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
func (l *lexer) peek() byte {
	if l.pos >= len(l.text) {
		return 0
	}
	return l.text[l.pos]
}

func (l *lexer) peekNext() byte {
	nextPos := l.pos + 1
	if nextPos >= len(l.text) {
		return 0
	}
	return l.text[nextPos]
}

func (l *lexer) advance() {
	l.pos++
}

func (l *lexer) skipSpace() {
	for isSpace(l.peek()) {
		l.advance()
	}
}

func (l *lexer) lexInteger() string {
	integer := ""
	for unicode.IsDigit(rune(l.peek())) {
		integer += string(l.peek())
		l.advance()
	}
	return integer
}

func (l *lexer) lexIdentifier() string {
	id := ""
	for unicode.IsLetter(rune(l.peek())) {
		id += string(l.peek())
		l.advance()
	}
	return id
}

func (l *lexer) currentToken() token {
	return l.tok
}

// case 1: skip whitespaces
// case 2: multiple digit
// case 3: supports + and -
// case 4: supports * and /
// case 5: supports ( and )
// case 6: supports BEGIN END DOT SEMI := and identifier
func (l *lexer) nextToken() {
	// Try to lexize each token in one loop.
	// For spaces, it will locate to the first non-space and restart lexize again.
	for l.pos < len(l.text) {
		ch := l.peek()

		if isSpace(ch) {
			l.skipSpace()
			continue
		}

		if unicode.IsDigit(rune(ch)) {
			value := l.lexInteger()
			l.tok = token{
				kind:  tokInteger,
				value: value,
			}
			return
		}

		if ch == '+' {
			l.tok = token{
				kind:  tokPlus,
				value: "+",
			}
			l.advance()
			return
		}

		if ch == '-' {
			l.tok = token{
				kind:  tokMinus,
				value: "-",
			}
			l.advance()
			return
		}

		if ch == '*' {
			l.advance()
			l.tok = token{
				kind:  tokMul,
				value: "*",
			}
			return
		}

		if ch == '/' {
			l.tok = token{
				kind:  tokDiv,
				value: "/",
			}
			l.advance()
			return
		}

		if ch == '(' {
			l.tok = token{
				kind:  tokLParen,
				value: "(",
			}
			l.advance()
			return
		}

		if ch == ')' {
			l.tok = token{
				kind:  tokRParen,
				value: ")",
			}
			l.advance()
			return
		}

		if unicode.IsLetter(rune(ch)) {
			id := l.lexIdentifier()
			keyword, in := _Reserved[id]
			if in {
				l.tok = *keyword
			} else {
				l.tok = token{
					kind:  tokVar,
					value: id,
				}
			}
			return
		}

		if ch == ';' {
			l.tok = token{
				kind:  tokSemi,
				value: ";",
			}
			l.advance()
			return
		}

		if ch == '.' {
			l.tok = token{
				kind:  tokDot,
				value: ".",
			}
			l.advance()
			return
		}

		if ch == ':' && l.peekNext() == '=' {
			l.tok = token{
				kind:  tokAssign,
				value: ":=",
			}
			l.advance()
			l.advance()
			return
		}

		l.tok = token{
			kind:  tokUnknown,
			value: string(ch),
		}
		return
	}

	l.tok = token{
		kind: tokEOF,
	}
}

func (l *lexer) eatToken(expectedKind int) (err error) {
	tok := l.currentToken()
	if tok.kind != expectedKind {
		return errors.Errorf("expected %d but got %+v", expectedKind, tok)
	}
	l.nextToken()
	return
}

// -*- parser -*-

type visitor interface {
	visitNum(*nodeNum) (int, error)
	visitBinOp(*nodeBinOp) (int, error)
	visitUnary(*nodeUnary) (int, error)
	visitNop(*nodeNop) (int, error)
	visitCompound(*nodeCompound) (int, error)
	visitAssign(*nodeAssign) (int, error)
	visitVariable(*nodeVariable) (int, error)
}

type visitable interface {
	doVisit(visitor) (int, error)
	value() string
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

func (n *nodeBinOp) value() string {
	return n.tok.value
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

func (n *nodeNum) value() string {
	return n.tok.value
}

type nodeUnary struct {
	tok token
	sub visitable
}

func newUnaryNode(t token, sub visitable) *nodeUnary {
	return &nodeUnary{
		tok: t,
		sub: sub,
	}
}

func (n *nodeUnary) doVisit(v visitor) (int, error) {
	return v.visitUnary(n)
}

func (n *nodeUnary) value() string {
	return n.tok.value
}

type nodeCompound struct {
	stmts []visitable
}

func newCompoundNode() *nodeCompound {
	return &nodeCompound{
		stmts: []visitable{},
	}
}

func (n *nodeCompound) doVisit(v visitor) (int, error) {
	return v.visitCompound(n)
}

func (n *nodeCompound) value() string {
	return "compound"
}

// left is an variable
// right is an expr
type nodeAssign struct {
	tok   token
	left  visitable
	right visitable
}

func newAssignNode(t token, variable, expr visitable) *nodeAssign {
	return &nodeAssign{
		tok:   t,
		left:  variable,
		right: expr,
	}
}

func (n *nodeAssign) doVisit(v visitor) (int, error) {
	return v.visitAssign(n)
}

func (n *nodeAssign) value() string {
	return n.tok.value
}

type nodeVariable struct {
	tok token
}

func newVariableNode(t token) *nodeVariable {
	return &nodeVariable{tok: t}
}

func (n *nodeVariable) doVisit(v visitor) (int, error) {
	return v.visitVariable(n)
}

func (n *nodeVariable) value() string {
	return n.tok.value
}

type nodeNop struct {
}

func newNopNode() *nodeNop {
	return &nodeNop{}
}

func (n *nodeNop) doVisit(v visitor) (int, error) {
	return v.visitNop(n)
}

func (n *nodeNop) value() string {
	return ""
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
		p.lexer.nextToken()
		node = newNumNode(tok)
	} else if tok.kind == tokPlus || tok.kind == tokMinus {
		p.lexer.nextToken()
		sub, err := p.consumeFactor()
		if err != nil {
			return nil, errors.Wrap(err, "consume factor for unary op")
		}
		node = newUnaryNode(tok, sub)
	} else if tok.kind == tokLParen {
		p.lexer.nextToken()
		// Handle sub-lexer recursively
		node, err = p.consumeExpr()
		if err != nil {
			return nil, errors.Wrap(err, "consume as expr")
		}
		if p.lexer.currentToken().kind != tokRParen {
			return nil, errors.Errorf("expected RPAREN but got %+v", p.lexer.currentToken())
		}
		p.lexer.nextToken()
	} else {
		node, err = p.consumeVariable()
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

// expr  := term ((plus|minus) term)*
// term   := factor ((mul|div) factor)*
// factor := integer | (plus|minus) factor | LPAREN lexer RPAREN
func (p *parser) consumeExpr() (node visitable, err error) {
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

// variable : var
func (p *parser) consumeVariable() (node visitable, err error) {
	node = newVariableNode(p.lexer.currentToken())
	if err = p.lexer.eatToken(tokVar); err != nil {
		return nil, err
	}
	return
}

// assignment_statement : variable ASSIGN expr
func (p *parser) consumeAssignment() (node visitable, err error) {
	varNode, err := p.consumeVariable()
	if err != nil {
		return nil, err
	}

	assignTok := p.lexer.currentToken()
	if err = p.lexer.eatToken(tokAssign); err != nil {
		return nil, err
	}

	exprNode, err := p.consumeExpr()
	if err != nil {
		return nil, err
	}

	node = newAssignNode(assignTok, varNode, exprNode)

	return
}

func (p *parser) consumeEmpty() (node visitable, err error) {
	return newNopNode(), nil
}

// statement : compound_statement
//           | assignment_statement
//           | empty
func (p *parser) consumeStatement() (node visitable, err error) {
	if p.lexer.currentToken().kind == tokBegin {
		node, err = p.consumeCompoundStatement()
		if err != nil {
			return nil, err
		}
	} else if p.lexer.currentToken().kind == tokVar {
		node, err = p.consumeAssignment()
		if err != nil {
			return nil, err
		}
	} else {
		node, err = p.consumeEmpty()
	}
	return
}

// statement_list : statement
//                | statement SEMI statement_list
func (p *parser) consumeStatementList() (nodes []visitable, err error) {
	node, err := p.consumeStatement()
	if err != nil {
		return nil, err
	}

	nodes = []visitable{node}

	for p.lexer.currentToken().kind == tokSemi {
		p.lexer.nextToken()
		node, err = p.consumeStatement()
		if err != nil {
			return nil, err
		}
		nodes = append(nodes, node)
	}

	return
}

// compound_statement : BEGIN statement_list END
func (p *parser) consumeCompoundStatement() (node visitable, err error) {
	if err = p.lexer.eatToken(tokBegin); err != nil {
		return nil, err
	}

	nodes, err := p.consumeStatementList()
	if err != nil {
		return nil, err
	}

	if err = p.lexer.eatToken(tokEnd); err != nil {
		return nil, err
	}

	compound := newCompoundNode()
	compound.stmts = nodes

	node = compound

	return
}

// program : compound_statement DOT
func (p *parser) consumeProgram() (node visitable, err error) {
	node, err = p.consumeCompoundStatement()
	if err != nil {
		return nil, err
	}

	err = p.lexer.eatToken(tokDot)
	if err != nil {
		return nil, err
	}

	return
}

func (p *parser) parse() (node visitable, err error) {
	node, err = p.consumeProgram()
	if err != nil {
		return nil, err
	}

	if p.lexer.currentToken().kind != tokEOF {
		return nil, errors.Errorf("expected EOF but got %+v", p.lexer.currentToken())
	}

	return
}

// -*- interpreter -*-

var (
	symbolTable = map[string]int{}
)

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

func (e *evaluator) visitUnary(node *nodeUnary) (result int, err error) {
	n, err := node.sub.doVisit(e)
	if node.tok.kind == tokPlus {
		result = n
	} else if node.tok.kind == tokMinus {
		result = -n
	}
	return
}

func (e *evaluator) visitNop(node *nodeNop) (int, error) {
	return 0, nil
}

func (e *evaluator) visitCompound(node *nodeCompound) (_ int, err error) {
	for _, st := range node.stmts {
		_, err := st.doVisit(e)
		if err != nil {
			return 0, err
		}
	}
	return
}

func (e *evaluator) visitAssign(node *nodeAssign) (_ int, err error) {
	varName := node.left.value()
	value, err := node.right.doVisit(e)
	if err != nil {
		return 0, err
	}

	symbolTable[varName] = value

	return
}

func (e *evaluator) visitVariable(node *nodeVariable) (value int, err error) {
	varName := node.value()
	value, in := symbolTable[varName]
	if !in {
		return 0, errors.Errorf("undefined variable %s", varName)
	}
	return
}

type interpreter struct {
	parser parser
	eval   evaluator
}

func newInterpreter(text string) interpreter {
	return interpreter{
		parser: newParser(text),
	}
}

func (i *interpreter) interpret() (int, error) {
	ast, err := i.parser.parse()
	if err != nil {
		return 0, err
	}
	return ast.doVisit(&i.eval)
}

// -*- run -*-

func main() {
	scanner := bufio.NewScanner(os.Stdin)
	for {
		fmt.Print("expr> ")
		if !scanner.Scan() {
			break
		}

		vm := newInterpreter(scanner.Text())
		_, err := vm.interpret()
		if err != nil {
			log.Printf("Error: %+v", err)
			continue
		}

		fmt.Printf("symbols: %+v\n", symbolTable)
	}
}
