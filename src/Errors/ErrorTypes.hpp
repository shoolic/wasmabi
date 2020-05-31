#ifndef WASMABI_ERROR_TYPES_HPP
#define WASMABI_ERROR_TYPES_HPP
#include "../Token/Token.hpp"
#include "ErrorTypes.hpp"
#include <iostream>
#include <map>
#include <string>
namespace wasmabi {

class GeneralError : public std::runtime_error {
public:
  GeneralError(std::string str);
};

class NoSourceFileError : public GeneralError {
public:
  NoSourceFileError();
};

class LexicalError : public std::runtime_error {
public:
  LexicalError(std::string str);
};

class UnexpSignAfterExclamationError : public LexicalError {
public:
  UnexpSignAfterExclamationError();
};

class UnexpectedZeroError : public LexicalError {
public:
  UnexpectedZeroError();
};

class ExtraDotError : public LexicalError {
public:
  ExtraDotError();
};

class ExpectedDigitError : public LexicalError {
public:
  ExpectedDigitError();
};

class InvalidTokenError : public LexicalError {
public:
  InvalidTokenError();
};

class NoCommentEndError : public LexicalError {
public:
  NoCommentEndError();
};

class NoQuoteEndError : public LexicalError {
public:
  NoQuoteEndError();
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

class GeneratorError : public std::runtime_error {
public:
  GeneratorError(std::string str);
};

class StringAsOperandError : public GeneratorError {
public:
  StringAsOperandError();
};

class UndefinedFunctionError : public GeneratorError {
public:
  UndefinedFunctionError(std::string identifier);
};

class UndefinedVariableError : public GeneratorError {
public:
  UndefinedVariableError(std::string identifier);
};

class VariableRedefintionError : public GeneratorError {
public:
  VariableRedefintionError(std::string identifier);
};

class FunctionReturnValueMismatchError : public GeneratorError {
public:
  FunctionReturnValueMismatchError();
};

class VoidFunretValueError : public GeneratorError {
public:
  VoidFunretValueError();
};

class UnpritableValueError : public GeneratorError {
public:
  UnpritableValueError();
};

class VarAssignTypeMismatch : public GeneratorError {
public:
  VarAssignTypeMismatch();
};

class StringAsConditionError : public GeneratorError {
public:
  StringAsConditionError();
};

} // namespace wasmabi

#endif // WASMABI_ERROR_TYPES_HPP
