#include "Token.hpp"

namespace wasmabi {

Token::Token() : type(Token::Type::Unknown) {}

const std::variant<std::string, int, float> Token::getValue() const {
  return value;
}

Token::Type Token::getType() const { return type; }

void Token::setType(Token::Type type_) {
  if (type != Type::Invalid) {
    type = type_;
  }
}

std::map<Token::Type, std::string> Token::map = {
    {Token::Type::Eof, "Eof"},
    {Token::Type::StringLiteral, "StringLiteral"},
    {Token::Type::IntLiteral, "IntLiteral"},
    {Token::Type::FloatLiteral, "FloatLiteral"},
    {Token::Type::StringTypename, "StringTypename"},
    {Token::Type::IntTypename, "IntTypename"},
    {Token::Type::FloatTypename, "FloatTypename"},
    {Token::Type::VoidTypename, "VoidTypename"},
    {Token::Type::Identifier, "Identifier"},
    {Token::Type::Semicolon, "Semicolon"},
    {Token::Type::OpeningParenthesis, "OpeningParenthesis"},
    {Token::Type::ClosingParenthesis, "ClosingParenthesis"},
    {Token::Type::OpeningBracket, "OpeningBracket"},
    {Token::Type::ClosingBracket, "ClosingBracket"},
    {Token::Type::Comma, "Comma"},
    {Token::Type::AssignmentOperator, "AssignmentOperator"},
    {Token::Type::TypeOperator, "TypeOperator"},
    {Token::Type::AddOperator, "AddOperator"},
    {Token::Type::SubOperator, "SubOperator"},
    {Token::Type::MulOperator, "MulOperator"},
    {Token::Type::DivOperator, "DivOperator"},
    {Token::Type::PowOperator, "PowOperator"},
    {Token::Type::AndOperator, "AndOperator"},
    {Token::Type::OrOperator, "OrOperator"},
    {Token::Type::NotOperator, "NotOperator"},
    {Token::Type::Equals, "Equals"},
    {Token::Type::NotEquals, "NotEquals"},
    {Token::Type::Greater, "Greater"},
    {Token::Type::GreaterOrEqual, "GreaterOrEqual"},
    {Token::Type::Less, "Less"},
    {Token::Type::LessOrEqual, "LessOrEqual"},
    {Token::Type::Select, "Select"},
    {Token::Type::Let, "Let"},
    {Token::Type::If, "If"},
    {Token::Type::Loop, "Loop"},
    {Token::Type::Print, "Print"},
    {Token::Type::Fn, "Fn"},
    {Token::Type::Return, "Return"},
    {Token::Type::When, "When"},
    {Token::Type::Otherwise, "Otherwise"},
    {Token::Type::Invalid, "Invalid"},
    {Token::Type::Unknown, "Unknown"},
}; // namespace wasmabi

std::ostream &operator<<(std::ostream &out, const Token::Type &type) {
  return out << Token::map[type];
}
} // namespace wasmabi
