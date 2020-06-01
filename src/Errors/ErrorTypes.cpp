#include "ErrorTypes.hpp"
#include "../Helpers/OstreamModifier.hpp"
#include <sstream>

namespace wasmabi {

GeneralError::GeneralError(std::string str) : std::runtime_error(str) {}

NoSourceFileError::NoSourceFileError()
    : GeneralError("Input file doesn't exist.") {}

LexicalError::LexicalError(std::string str) : std::runtime_error(str) {}

UnexpSignAfterExclamationError::UnexpSignAfterExclamationError()
    : LexicalError("Expected !=") {}

UnexpectedZeroError::UnexpectedZeroError()
    : LexicalError(
          "Incorrect number format (unexpected zero at the beginning?)") {}

ExtraDotError::ExtraDotError()
    : LexicalError("Incorrect number format (extra dot?)") {}

ExpectedDigitError::ExpectedDigitError()
    : LexicalError("Incorrect number format (digit expected)") {}

InvalidTokenError::InvalidTokenError() : LexicalError("Invalid token") {}

NoCommentEndError::NoCommentEndError() : LexicalError("No comment end") {}

NoQuoteEndError::NoQuoteEndError() : LexicalError("No quote end") {}

SyntaxError::SyntaxError(Token got_) : got(got_) {}

const char *SyntaxError::what() { return "syntax error\n"; }

UnexpectedToken::UnexpectedToken(Token got_, Token::Type expected_)
    : SyntaxError(got_), expected(expected_) {}

const char *UnexpectedToken::what() {
  std::stringstream message;
  message << "Expected " << expected << ", but got " << got.getType();
  return message.str().c_str();
}

UnknownFunRetType::UnknownFunRetType(Token got_) : SyntaxError(got_) {}
const char *UnknownFunRetType::what() {
  return "Incorrect function return type\n";
}

UnknownVarRetType::UnknownVarRetType(Token got_) : SyntaxError(got_) {}
const char *UnknownVarRetType::what() { return "Incorrect variable type\n"; }

StatementError::StatementError(Token got_) : SyntaxError(got_) {}
const char *StatementError::what() {
  return "Unknown token at the beginning of the statement\n";
}

StatmentWithIdentifierError::StatmentWithIdentifierError(Token got_)
    : SyntaxError(got_) {}
const char *StatmentWithIdentifierError::what() {
  return "Found an identifier at the beginning of the statement, but failed to "
         "recognize a futher statement type\n";
}

VariableDefinitionStatementError::VariableDefinitionStatementError(Token got_)
    : SyntaxError(got_) {}
const char *VariableDefinitionStatementError::what() {
  return "Unexpected token while parsing VariableDefinitionStatementError\n";
}

SelectExpressionError::SelectExpressionError(Token got_) : SyntaxError(got_) {}
const char *SelectExpressionError::what() {
  return "Expected when or otherwise keywords\n";
}

ValueExpressionNudError::ValueExpressionNudError(Token got_)
    : SyntaxError(got_) {}
const char *ValueExpressionNudError::what() {
  return "Failed to identify primary node while parsing ValueExpression\n";
}

ValueExpressionLedError::ValueExpressionLedError(Token got_)
    : SyntaxError(got_) {}
const char *ValueExpressionLedError::what() {
  return "No right operand found\n";
}

UnexpectedTokenValExprError::UnexpectedTokenValExprError(Token got_)
    : SyntaxError(got_) {}
const char *UnexpectedTokenValExprError::what() {
  return "Unexpected token while parsing ValueExpression\n";
}

GeneratorError::GeneratorError(std::string str) : std::runtime_error(str) {}

StringAsOperandError::StringAsOperandError()
    : GeneratorError("Cannot use string value as operand.") {}

UndefinedFunctionError::UndefinedFunctionError(std::string identifier)
    : GeneratorError("Function with " + identifier + " identifier not found.") {
}

UndefinedVariableError::UndefinedVariableError(std::string identifier)
    : GeneratorError("Variable with " + identifier + " identifier not found.") {
}

VariableRedefintionError::VariableRedefintionError(std::string identifier)
    : GeneratorError("Cannot redefine variable with " + identifier +
                     " identifier") {}

FunctionReturnValueMismatchError::FunctionReturnValueMismatchError()
    : GeneratorError("Function returned type and value type does not match.") {}

VoidFunretValueError::VoidFunretValueError()
    : GeneratorError("Function of type void cannot return value.") {}

UnpritableValueError::UnpritableValueError()
    : GeneratorError("Cannot print variable of this type.") {}

VarAssignTypeMismatch::VarAssignTypeMismatch()
    : GeneratorError("Type of value does not match variable type.") {}

StringAsConditionError::StringAsConditionError()
    : GeneratorError("String cannot be used in condition.") {}

} // namespace wasmabi