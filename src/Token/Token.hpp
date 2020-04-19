#ifndef WASMABI_TOKEN_HPP
#define WASMABI_TOKEN_HPP

#include <iostream>
#include <map>
#include <string>
namespace wasmabi {

class Token {
public:
  Token();
  enum class Type {
    Eof,
    StringLiteral,
    IntLiteral,
    FloatLiteral,

    StringTypename,
    IntTypename,
    FloatTypename,
    VoidTypename,

    Identifier,

    Semicolon,

    OpeningParenthesis,
    ClosingParenthesis,

    OpeningBracket,
    ClosingBracket,

    Comma,

    AssignmentOperator,
    TypeOperator,

    AddOperator,
    SubOperator,
    MulOperator,
    DivOperator,
    PowOperator,

    AndOperator,
    OrOperator,
    NotOperator,

    Equals,
    NotEquals,
    Greater,
    GreaterOrEqual,
    Less,
    LessOrEqual,

    Select,
    Let,
    If,
    Loop,
    Print,
    Fn,
    Return,
    When,
    Otherwise,

    Invalid
  };

  struct potentiallyDoubleChar {
    char current;
    char next;
    Type typeIfNextNotMatched;
    Type typeIfNextMatched;
  };

  static std::map<Type, std::string> map;
  friend std::ostream &operator<<(std::ostream &os, const Token::Type &dt);

  std::string value;
  Type type;
  const std::string getValue() const;
  Token::Type getType() const;

private:
};

} // namespace wasmabi
#endif // WASMABI_TOKEN_HPP
