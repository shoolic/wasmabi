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
  SyntaxError();
};

class UnexpectedToken : public SyntaxError {
public:
  UnexpectedToken(Token got, Token::Type expected);
};

class UnknownFunRetType : public SyntaxError {
public:
  UnknownFunRetType(Token got);
};

class UnknownVarRetType : public SyntaxError {
public:
  UnknownVarRetType(Token got);
};

class StatementError : public SyntaxError {
public:
  StatementError(Token got);
};

class StatmentWithIdentifierError : public SyntaxError {
public:
  StatmentWithIdentifierError(Token got);
};

class VariableDefinitionStatementError : public SyntaxError {
public:
  VariableDefinitionStatementError(Token got);
};

class SelectExpressionError : public SyntaxError {
public:
  SelectExpressionError(Token got);
};

class ValueExpressionNudError : public SyntaxError {
public:
  ValueExpressionNudError(Token got);
};

class ValueExpressionLedError : public SyntaxError {
public:
  ValueExpressionLedError(Token got);
};

} // namespace wasmabi

#endif // WASMABI_ERROR_TYPES_HPP
