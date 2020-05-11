#include "ErrorTypes.hpp"
#include "../helpers/OstreamModifier.hpp"
#include <sstream>

namespace wasmabi {

std::map<Error::Lexical, std::string> Error::lexicalMap = {
    {Error::Lexical::UnexpectedZero,
     "Incorrect number format (unexpected zero at the beginning?)"},
    {Error::Lexical::ExtraDot, "Incorrect number format (extra dot?)"},
    {Error::Lexical::ExpectedDigit, "Incorrect number format (digit expected)"},
    {Error::Lexical::InvalidToken, "Invalid token"},
    {Error::Lexical::NoCommentEnd, "No comment end"},
    {Error::Lexical::NoQuoteEnd, "No quote end"},
};

std::ostream &operator<<(std::ostream &os, const Error::Lexical &err) {
  os << Error::lexicalMap[err];
  return os;
}

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
  // std::stringstream message;
  // message << "Incorrect function type, got " << got.getType();
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
  return "Failed to build BinaryExpression\n";
}

UnexpectedTokenValExprError::UnexpectedTokenValExprError(Token got_)
    : SyntaxError(got_) {}
const char *UnexpectedTokenValExprError::what() {
  return "Unexpected token while parsing ValueExpression\n";
}

} // namespace wasmabi