#include "Lexer.hpp"
#include "../Errors/ErrorTypes.hpp"
#include <iostream>
namespace wasmabi {
Lexer::Lexer(SourceController &sourceController_, ErrorHandler &errorHandler_)
    : sourceController(sourceController_), errorHandler(errorHandler_),
      entryFunctions{
          [this](Token &token) -> bool { return trySkipComment(token); },
          [this](Token &token) -> bool { return tryEnd(token); },
          [this](Token &token) -> bool { return tryStringLiteral(token); },
          [this](Token &token) -> bool { return tryNumericLiteral(token); },
          [this](Token &token) -> bool { return trySingleCharToken(token); },
          [this](Token &token) -> bool { return tryDoubleCharToken(token); },
          [this](Token &token) -> bool {
            return tryIdentifierOrKeyword(token);
          },
      } {}

const Token Lexer::nextToken() {
  Token token;

  for (auto &tryEntryFunction : entryFunctions) {
    skipWhitespaces();
    if (tryEntryFunction(token)) {
      return token;
    }
  }

  errorHandler.registerLexicalError(Error::Lexical::InvalidToken);

  token.setType(Token::Type::Invalid);
  return token;
}

void Lexer::skipWhitespaces() {
  while (isspace(sourceController.peek())) {
    sourceController.get();
  }
}

bool Lexer::trySkipComment(Token &token) {
  if (sourceController.peek() == Boundaries::Comment) {
    sourceController.get();

    while (sourceController.get() != Boundaries::Comment) {
      if (sourceController.end()) {
        errorHandler.registerLexicalError(Error::Lexical::NoCommentEnd);
        token.setType(Token::Type::Invalid);

        return true;
      }
    };
  }

  return false;
}

bool Lexer::tryEnd(Token &token) {
  if (sourceController.end()) {
    token.setType(Token::Type::Eof);
    return true;
  }

  return false;
}

bool Lexer::tryStringLiteral(Token &token) {
  std::string value;

  if (sourceController.peek() == Boundaries::Quote) {
    token.setType(Token::Type::StringLiteral);
    sourceController.get();
    char c = '\0';

    while ((c = sourceController.get()) != Boundaries::Quote) {
      value += c;
      if (sourceController.end()) {
        errorHandler.registerLexicalError(Error::Lexical::NoQuoteEnd);
        token.value = value;
        token.setType(Token::Type::Invalid);
        return true;
      }
    }

    token.value = value;
    return true;
  }

  return false;
}

bool Lexer::tryNumericLiteral(Token &token) {
  if (!isdigit(sourceController.peek())) {
    return false;
  }

  std::string value;
  token.setType(Token::Type::IntLiteral);
  char c = sourceController.get();
  value += c;

  if (c == '0' && isdigit(sourceController.peek())) {
    token.setType(Token::Type::Invalid);
    token.value = value;
    errorHandler.registerLexicalError(Error::Lexical::UnexpectedZero);
    return true;
  }

  while (true) {
    if (sourceController.peek() == '.') {
      if (token.type == Token::Type::FloatLiteral) {
        errorHandler.registerLexicalError(Error::Lexical::ExtraDot);
        token.setType(Token::Type::Invalid);
        token.value = value;

        return true;
      } else {
        token.setType(Token::Type::FloatLiteral);
        value += sourceController.get();
        if (!isdigit(sourceController.peek())) {
          errorHandler.registerLexicalError(Error::Lexical::ExpectedDigit);
          token.setType(Token::Type::Invalid);
          token.value = value;

          return true;
        }
      }
    } else if (!isdigit(sourceController.peek())) {

      if (token.type == Token::Type::FloatLiteral) {
        token.value = float(std::atof(value.c_str()));
      } else if (token.type == Token::Type::IntLiteral) {
        token.value = std::atoi(value.c_str());
      }

      return true;
    }

    value += sourceController.get();
  }

  return true;
}

bool Lexer::trySingleCharToken(Token &token) {
  auto it = singleCharMap.find(sourceController.peek());

  if (it != singleCharMap.end()) {
    token.value = std::string(1, sourceController.get());
    token.setType(it->second);
    return true;
  }

  return false;
}

bool Lexer::tryDoubleCharToken(Token &token) {
  auto it = potentiallyDoubleCharMap.find(sourceController.peek());
  std::string value;

  if (it != potentiallyDoubleCharMap.end()) {
    value += sourceController.get();

    if (sourceController.peek() == it->second.next) {
      value += sourceController.get();

      token.value = value;
      token.setType(it->second.typeIfNextMatched);
      return true;
    }

    token.value = value;
    token.setType(it->second.typeIfNextNotMatched);
    return true;
  }

  return false;
}

bool Lexer::tryIdentifierOrKeyword(Token &token) {

  std::string value;

  if (isIdentifierChar(sourceController.peek())) {
    token.setType(Token::Type::Identifier);

    while (isIdentifierChar(sourceController.peek())) {
      value += sourceController.get();
    }

    token.value = value;

    tryKeyword(token);

    return true;
  }

  return false;
}

bool Lexer::tryKeyword(Token &token) {
  auto it = keywords.find(std::get<std::string>(token.value));
  if (it != keywords.end()) {
    token.setType(it->second);
    return true;
  }
  return false;
}

bool Lexer::isIdentifierChar(char c) { return isalnum(c) || c == '_'; }

std::map<std::string, Token::Type> Lexer::keywords = {
    {"select", Token::Type::Select},
    {"let", Token::Type::Let},
    {"if", Token::Type::If},
    {"loop", Token::Type::Loop},
    {"print", Token::Type::Print},
    {"fn", Token::Type::Fn},
    {"return", Token::Type::Return},
    {"when", Token::Type::When},
    {"otherwise", Token::Type::Otherwise},
    {"and", Token::Type::AndOperator},
    {"or", Token::Type::OrOperator},
    {"not", Token::Type::NotOperator},
    {"int", Token::Type::IntTypename},
    {"float", Token::Type::FloatTypename},
    {"string", Token::Type::StringTypename},
    {"void", Token::Type::VoidTypename}}; // namespace wasmabi

std::map<char, Token::Type> Lexer::singleCharMap = {
    {'}', Token::Type::ClosingBracket},
    {'{', Token::Type::OpeningBracket},
    {')', Token::Type::ClosingParenthesis},
    {'(', Token::Type::OpeningParenthesis},
    {'+', Token::Type::AddOperator},
    {'-', Token::Type::SubOperator},
    {'*', Token::Type::MulOperator},
    {'/', Token::Type::DivOperator},
    {'^', Token::Type::PowOperator},
    {':', Token::Type::TypeOperator},
    {',', Token::Type::Comma},
    {';', Token::Type::Semicolon},

}; // namespace wasmabi

std::map<char, Lexer::potentiallyDoubleChar> Lexer::potentiallyDoubleCharMap = {
    {'<', {'<', '=', Token::Type::Less, Token::Type::LessOrEqual}},
    {'>', {'>', '=', Token::Type::Greater, Token::Type::GreaterOrEqual}},
    {'=', {'=', '=', Token::Type::AssignmentOperator, Token::Type::Equals}},
    {'!', {'!', '=', Token::Type::NotEquals, Token::Type::NotEquals}},
};
} // namespace wasmabi