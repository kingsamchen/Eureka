package main

import (
	"testing"

	"github.com/smartystreets/goconvey/convey"
)

func TestMain(m *testing.M) {
	m.Run()
}

func TestLexer_PascalDefinitions(t *testing.T) {
	convey.Convey("Get tokens for pascal definitions", t, func(c convey.C) {
		lex := newLexer("BEGIN foo := 2; END.")
		var tok token

		tok = lex.currentToken()
		c.So(tok.kind, convey.ShouldEqual, tokBegin)
		c.So(tok.value, convey.ShouldEqual, "BEGIN")

		lex.nextToken()
		tok = lex.currentToken()
		c.So(tok.kind, convey.ShouldEqual, tokSymbol)
		c.So(tok.value, convey.ShouldEqual, "foo")

		lex.nextToken()
		tok = lex.currentToken()
		c.So(tok.kind, convey.ShouldEqual, tokAssign)
		c.So(tok.value, convey.ShouldEqual, ":=")

		lex.nextToken()
		tok = lex.currentToken()
		c.So(tok.kind, convey.ShouldEqual, tokLiteralInteger)
		c.So(tok.value, convey.ShouldEqual, "2")

		lex.nextToken()
		tok = lex.currentToken()
		c.So(tok.kind, convey.ShouldEqual, tokSemi)
		c.So(tok.value, convey.ShouldEqual, ";")

		lex.nextToken()
		tok = lex.currentToken()
		c.So(tok.kind, convey.ShouldEqual, tokEnd)
		c.So(tok.value, convey.ShouldEqual, "END")

		lex.nextToken()
		tok = lex.currentToken()
		c.So(tok.kind, convey.ShouldEqual, tokDot)
		c.So(tok.value, convey.ShouldEqual, ".")

		lex.nextToken()
		c.So(lex.currentToken().kind, convey.ShouldEqual, tokEOF)
	})

	convey.Convey("Identifiers can start with _", t, func(c convey.C) {
		lex := newLexer("BEGIN _foo_bar := 2; END")
		var tok token

		tok = lex.currentToken()
		c.So(tok, convey.ShouldResemble, *_Reserved["BEGIN"])

		lex.nextToken()
		tok = lex.currentToken()
		c.So(tok, convey.ShouldResemble, token{kind: tokSymbol, value: "_foo_bar"})

		lex.nextToken()
		tok = lex.currentToken()
		c.So(tok, convey.ShouldResemble, token{kind: tokAssign, value: ":="})
	})

	convey.Convey("", t, func(c convey.C) {
		text :=
			`PROGRAM test;
VAR
	number : INTEGER;
	x, y, z : INTEGER;
	gg : REAL;
BEGIN
	z := x / y;
	gg := number // z;
END.`
		lex := newLexer(text)
		var tok token

		tok = lex.currentToken()
		c.So(tok, convey.ShouldResemble, *_Reserved["PROGRAM"])
		lex.nextToken()

		tok = lex.currentToken()
		c.So(tok, convey.ShouldResemble, token{kind: tokSymbol, value: "test"})
		lex.nextToken()

		tok = lex.currentToken()
		c.So(tok, convey.ShouldResemble, token{kind: tokSemi, value: ";"})
		lex.nextToken()

		tok = lex.currentToken()
		c.So(tok, convey.ShouldResemble, *_Reserved["VAR"])
		lex.nextToken()

		tok = lex.currentToken()
		c.So(tok, convey.ShouldResemble, token{kind: tokSymbol, value: "number"})
		lex.nextToken()

		tok = lex.currentToken()
		c.So(tok, convey.ShouldResemble, token{kind: tokColon, value: ":"})
		lex.nextToken()

		tok = lex.currentToken()
		c.So(tok, convey.ShouldResemble, *_Reserved["INTEGER"])
		lex.nextToken()

		tok = lex.currentToken()
		c.So(tok, convey.ShouldResemble, token{kind: tokSemi, value: ";"})
		lex.nextToken()

		tok = lex.currentToken()
		c.So(tok, convey.ShouldResemble, token{kind: tokSymbol, value: "x"})
		lex.nextToken()

		tok = lex.currentToken()
		c.So(tok, convey.ShouldResemble, token{kind: tokComma, value: ","})
		lex.nextToken()

		tok = lex.currentToken()
		c.So(tok, convey.ShouldResemble, token{kind: tokSymbol, value: "y"})
		lex.nextToken()

		tok = lex.currentToken()
		c.So(tok, convey.ShouldResemble, token{kind: tokComma, value: ","})
		lex.nextToken()

		tok = lex.currentToken()
		c.So(tok, convey.ShouldResemble, token{kind: tokSymbol, value: "z"})
		lex.nextToken()

		tok = lex.currentToken()
		c.So(tok, convey.ShouldResemble, token{kind: tokColon, value: ":"})
		lex.nextToken()

		tok = lex.currentToken()
		c.So(tok, convey.ShouldResemble, *_Reserved["INTEGER"])
		lex.nextToken()

		tok = lex.currentToken()
		c.So(tok, convey.ShouldResemble, token{kind: tokSemi, value: ";"})
		lex.nextToken()

		tok = lex.currentToken()
		c.So(tok, convey.ShouldResemble, token{kind: tokSymbol, value: "gg"})
		lex.nextToken()

		tok = lex.currentToken()
		c.So(tok, convey.ShouldResemble, token{kind: tokColon, value: ":"})
		lex.nextToken()

		tok = lex.currentToken()
		c.So(tok, convey.ShouldResemble, *_Reserved["REAL"])
		lex.nextToken()

		tok = lex.currentToken()
		c.So(tok, convey.ShouldResemble, token{kind: tokSemi, value: ";"})
		lex.nextToken()

		tok = lex.currentToken()
		c.So(tok, convey.ShouldResemble, *_Reserved["BEGIN"])
		lex.nextToken()

		tok = lex.currentToken()
		c.So(tok, convey.ShouldResemble, token{kind: tokSymbol, value: "z"})
		lex.nextToken()

		tok = lex.currentToken()
		c.So(tok, convey.ShouldResemble, token{kind: tokAssign, value: ":="})
		lex.nextToken()

		tok = lex.currentToken()
		c.So(tok, convey.ShouldResemble, token{kind: tokSymbol, value: "x"})
		lex.nextToken()

		tok = lex.currentToken()
		c.So(tok, convey.ShouldResemble, token{kind: tokDivInteger, value: "/"})
		lex.nextToken()

		tok = lex.currentToken()
		c.So(tok, convey.ShouldResemble, token{kind: tokSymbol, value: "y"})
		lex.nextToken()

		tok = lex.currentToken()
		c.So(tok, convey.ShouldResemble, token{kind: tokSemi, value: ";"})
		lex.nextToken()

		tok = lex.currentToken()
		c.So(tok, convey.ShouldResemble, token{kind: tokSymbol, value: "gg"})
		lex.nextToken()

		tok = lex.currentToken()
		c.So(tok, convey.ShouldResemble, token{kind: tokAssign, value: ":="})
		lex.nextToken()

		tok = lex.currentToken()
		c.So(tok, convey.ShouldResemble, token{kind: tokSymbol, value: "number"})
		lex.nextToken()

		tok = lex.currentToken()
		c.So(tok, convey.ShouldResemble, token{kind: tokDivReal, value: "//"})
		lex.nextToken()

		tok = lex.currentToken()
		c.So(tok, convey.ShouldResemble, token{kind: tokSymbol, value: "z"})
		lex.nextToken()

		tok = lex.currentToken()
		c.So(tok, convey.ShouldResemble, token{kind: tokSemi, value: ";"})
		lex.nextToken()

		tok = lex.currentToken()
		c.So(tok, convey.ShouldResemble, *_Reserved["END"])
		lex.nextToken()

		tok = lex.currentToken()
		c.So(tok, convey.ShouldResemble, token{kind: tokDot, value: "."})
		lex.nextToken()
	})
}

func TestParser_PascalDefinitions(t *testing.T) {
	convey.Convey("", t, func(c convey.C) {
		parser := newParser("BEGIN foo := 2; END.")
		node, err := parser.parse()
		c.So(err, convey.ShouldBeNil)
		_, ok := node.(*nodeCompound)
		c.So(ok, convey.ShouldBeTrue)
	})
}
