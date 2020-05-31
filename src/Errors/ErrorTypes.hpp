#ifndef WASMABI_ERROR_TYPES_HPP
#define WASMABI_ERROR_TYPES_HPP
#include "../Token/Token.hpp"
#include "ErrorTypes.hpp"
#include <iostream>
#include <map>
#include <string>
namespace wasmabi {

class Error {
public:
  enum class Lexical {
    UnexpectedZero,
    ExtraDot,
    ExpectedDigit,
    InvalidToken,
    NoCommentEnd,
    NoQuoteEnd,
  };

private:
  static std::map<Lexical, std::string> lexicalMap;
  friend std::ostream &operator<<(std::ostream &os, const Error::Lexical &err);
};

class SyntaxError : public std::exception {
public:
  SyntaxError(Token got_);
  Token got;
  virtual const char *what();
};

class UnexpectedToken : public SyntaxError {
public:
  UnexpectedToken(Token got_, Token::Type expected_);
  const char *what();
  Token::Type expected;
};

class UnknownFunRetType : public SyntaxError {
public:
  UnknownFunRetType(Token got_);
  const char *what();
};

class UnknownVarRetType : public SyntaxError {
public:
  UnknownVarRetType(Token got_);
  const char *what();
};

class StatementError : public SyntaxError {
public:
  StatementError(Token got_);
  const char *what();
};

class StatmentWithIdentifierError : public SyntaxError {
public:
  StatmentWithIdentifierError(Token got_);
  const char *what();
};

class VariableDefinitionStatementError : public SyntaxError {
public:
  VariableDefinitionStatementError(Token got_);
  const char *what();
};

class SelectExpressionError : public SyntaxError {
public:
  SelectExpressionError(Token got_);
  const char *what();
};

class ValueExpressionNudError : public SyntaxError {
public:
  ValueExpressionNudError(Token got_);
  const char *what();
};

class ValueExpressionLedError : public SyntaxError {
public:
  ValueExpressionLedError(Token got_);
  const char *what();
};

class UnexpectedTokenValExprError : public SyntaxError {
public:
  UnexpectedTokenValExprError(Token got_);
  const char *what();
};

class GeneratorError : public std::runtime_error {};

} // namespace wasmabi

#endif // WASMABI_ERROR_TYPES_HPP
