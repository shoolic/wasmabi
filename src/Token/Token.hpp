#ifndef WASMABI_TOKEN_HPP
#define WASMABI_TOKEN_HPP

#include <iostream>
#include <map>
#include <string>
#include <variant>

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

    Invalid,
    Unknown
  };

  struct potentiallyDoubleChar {
    char current;
    char next;
    Type typeIfNextNotMatched;
    Type typeIfNextMatched;
  };

  static std::map<Type, std::string> map;
  friend std::ostream &operator<<(std::ostream &os, const Token::Type &dt);

  const std::variant<std::string, int, float> getValue() const;
  Token::Type getType() const;
  void setType(Token::Type type);

  friend class Lexer;

private:
  Type type;
  std::variant<std::string, int, float> value;
};

} // namespace wasmabi
#endif // WASMABI_TOKEN_HPP
