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
		c.So(tok.kind, convey.ShouldEqual, tokVar)
		c.So(tok.value, convey.ShouldEqual, "foo")

		lex.nextToken()
		tok = lex.currentToken()
		c.So(tok.kind, convey.ShouldEqual, tokAssign)
		c.So(tok.value, convey.ShouldEqual, ":=")

		lex.nextToken()
		tok = lex.currentToken()
		c.So(tok.kind, convey.ShouldEqual, tokInteger)
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
		c.So(tok, convey.ShouldResemble, token{kind: tokVar, value:"_foo_bar"})

		lex.nextToken()
		tok = lex.currentToken()
		c.So(tok, convey.ShouldResemble, token{kind: tokAssign, value:":="})
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
