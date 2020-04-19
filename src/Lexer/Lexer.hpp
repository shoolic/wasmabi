#ifndef WASMABI_LEXER_HPP
#define WASMABI_LEXER_HPP
#include "../Errors/ErrorHandler.hpp"
#include "../SourceController/SourceController.hpp"
#include "../Token/Token.hpp"
#include <map>
namespace wasmabi {

class Lexer {
public:
  Lexer(SourceController &sourceController_, ErrorHandler &errorHandler_);
  const Token nextToken();

private:
  SourceController &sourceController;
  ErrorHandler &errorHandler;

  void skipWhitespaces();
  bool trySkipComment(Token &token);
  bool tryStringLiteral(Token &token);
  bool tryNumericLiteral(Token &token);
  bool trySingleCharToken(Token &token);
  bool tryDoubleCharToken(Token &token);
  bool tryKeyword(Token &token);
  bool tryIdentifierOrKeyword(Token &token);
  bool isIdentifierChar(char c);

  static struct Boundaries {
    static const char Comment = '@', Quote = '\"';
  } Boundaries;

  struct potentiallyDoubleChar {
    char current;
    char next;
    Token::Type typeIfNextNotMatched;
    Token::Type typeIfNextMatched;
  };

  static std::map<std::string, Token::Type> keywords;
  static std::map<char, Token::Type> singleCharMap;
  static std::map<char, potentiallyDoubleChar> potentiallyDoubleCharMap;
};

} // namespace wasmabi

#endif // WASMABI_LEXER_HPP
