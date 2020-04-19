#include "Lexer/Lexer.hpp"
#include "SourceController/SourceController.hpp"
#include "Token/Token.hpp"
#include <boost/test/unit_test.hpp>

#include "helpers/NullOstream.hpp"
#include <filesystem>
#include <fstream>
#include <string>
using namespace wasmabi;

BOOST_AUTO_TEST_SUITE(PreDocsExample1)

OSTREAM_DEFINITION

BOOST_AUTO_TEST_CASE(PreDocsExample1) {
  std::string path("tests/examples/preDocs1.wa");
  std::ifstream file;
  file.open(path);

  SourceController sourceController(file,
                                    std::filesystem::absolute(path).string());
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);

  // vvvvvv FUNCTION HEADER vvvvvv
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Fn);
    BOOST_CHECK_EQUAL(token.getValue(), "fn");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Identifier);
    BOOST_CHECK_EQUAL(token.getValue(), "calc");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::OpeningParenthesis);
    BOOST_CHECK_EQUAL(token.getValue(), "(");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Identifier);
    BOOST_CHECK_EQUAL(token.getValue(), "base");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::TypeOperator);
    BOOST_CHECK_EQUAL(token.getValue(), ":");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::IntTypename);
    BOOST_CHECK_EQUAL(token.getValue(), "int");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Comma);
    BOOST_CHECK_EQUAL(token.getValue(), ",");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Identifier);
    BOOST_CHECK_EQUAL(token.getValue(), "exponent");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::TypeOperator);
    BOOST_CHECK_EQUAL(token.getValue(), ":");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::IntTypename);
    BOOST_CHECK_EQUAL(token.getValue(), "int");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::ClosingParenthesis);
    BOOST_CHECK_EQUAL(token.getValue(), ")");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::TypeOperator);
    BOOST_CHECK_EQUAL(token.getValue(), ":");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::IntTypename);
    BOOST_CHECK_EQUAL(token.getValue(), "int");
  }
  // ^^^^^^ CALL FUNCTION HEADER ^^^^^^
  // vvvvvv CALL FUNCTION BODY vvvvvv

  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::OpeningBracket);
    BOOST_CHECK_EQUAL(token.getValue(), "{");
  }

  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Return);
    BOOST_CHECK_EQUAL(token.getValue(), "return");
  }

  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Select);
    BOOST_CHECK_EQUAL(token.getValue(), "select");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::OpeningBracket);
    BOOST_CHECK_EQUAL(token.getValue(), "{");
  }

  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Identifier);
    BOOST_CHECK_EQUAL(token.getValue(), "base");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::PowOperator);
    BOOST_CHECK_EQUAL(token.getValue(), "^");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Identifier);
    BOOST_CHECK_EQUAL(token.getValue(), "exponent");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::AddOperator);
    BOOST_CHECK_EQUAL(token.getValue(), "+");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::IntLiteral);
    BOOST_CHECK_EQUAL(token.getValue(), "1");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::When);
    BOOST_CHECK_EQUAL(token.getValue(), "when");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Identifier);
    BOOST_CHECK_EQUAL(token.getValue(), "exponent");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Greater);
    BOOST_CHECK_EQUAL(token.getValue(), ">");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::IntLiteral);
    BOOST_CHECK_EQUAL(token.getValue(), "0");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::AndOperator);
    BOOST_CHECK_EQUAL(token.getValue(), "and");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::NotOperator);
    BOOST_CHECK_EQUAL(token.getValue(), "not");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Identifier);
    BOOST_CHECK_EQUAL(token.getValue(), "base");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Less);
    BOOST_CHECK_EQUAL(token.getValue(), "<");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::IntLiteral);
    BOOST_CHECK_EQUAL(token.getValue(), "0");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Comma);
    BOOST_CHECK_EQUAL(token.getValue(), ",");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::IntLiteral);
    BOOST_CHECK_EQUAL(token.getValue(), "0");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Otherwise);
    BOOST_CHECK_EQUAL(token.getValue(), "otherwise");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::ClosingBracket);
    BOOST_CHECK_EQUAL(token.getValue(), "}");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Semicolon);
    BOOST_CHECK_EQUAL(token.getValue(), ";");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::ClosingBracket);
    BOOST_CHECK_EQUAL(token.getValue(), "}");
  }
  // ^^^^^^ CALL FUNCTION BODY ^^^^^^
  // vvvvvv PRINTER FUNCTION HEADER vvvvvv
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Fn);
    BOOST_CHECK_EQUAL(token.getValue(), "fn");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Identifier);
    BOOST_CHECK_EQUAL(token.getValue(), "printer");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::OpeningParenthesis);
    BOOST_CHECK_EQUAL(token.getValue(), "(");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::ClosingParenthesis);
    BOOST_CHECK_EQUAL(token.getValue(), ")");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::TypeOperator);
    BOOST_CHECK_EQUAL(token.getValue(), ":");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::VoidTypename);
    BOOST_CHECK_EQUAL(token.getValue(), "void");
  }
  // ^^^^^^ PRINTER FUNCTION HEADER ^^^^^^

  // vvvvvv PRINTER FUNCTION BODY vvvvvv
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::OpeningBracket);
    BOOST_CHECK_EQUAL(token.getValue(), "{");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Print);
    BOOST_CHECK_EQUAL(token.getValue(), "print");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::StringLiteral);
    BOOST_CHECK_EQUAL(token.getValue(), "tekst");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Semicolon);
    BOOST_CHECK_EQUAL(token.getValue(), ";");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::ClosingBracket);
    BOOST_CHECK_EQUAL(token.getValue(), "}");
  }
  // ^^^^^^ PRINTER FUNCTION BODY ^^^^^^

  // vvvvvv MAIN FUNCTION HEADER vvvvvv
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Fn);
    BOOST_CHECK_EQUAL(token.getValue(), "fn");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Identifier);
    BOOST_CHECK_EQUAL(token.getValue(), "main");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::OpeningParenthesis);
    BOOST_CHECK_EQUAL(token.getValue(), "(");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::ClosingParenthesis);
    BOOST_CHECK_EQUAL(token.getValue(), ")");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::TypeOperator);
    BOOST_CHECK_EQUAL(token.getValue(), ":");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::IntTypename);
    BOOST_CHECK_EQUAL(token.getValue(), "int");
  }
  // ^^^^^^ MAIN FUNCTION HEADER ^^^^^^

  // vvvvvv MAIN FUNCTION BODY vvvvvv
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::OpeningBracket);
    BOOST_CHECK_EQUAL(token.getValue(), "{");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Let);
    BOOST_CHECK_EQUAL(token.getValue(), "let");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Identifier);
    BOOST_CHECK_EQUAL(token.getValue(), "x");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::TypeOperator);
    BOOST_CHECK_EQUAL(token.getValue(), ":");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::IntTypename);
    BOOST_CHECK_EQUAL(token.getValue(), "int");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::AssignmentOperator);
    BOOST_CHECK_EQUAL(token.getValue(), "=");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Identifier);
    BOOST_CHECK_EQUAL(token.getValue(), "calc");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::OpeningParenthesis);
    BOOST_CHECK_EQUAL(token.getValue(), "(");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::IntLiteral);
    BOOST_CHECK_EQUAL(token.getValue(), "2");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Comma);
    BOOST_CHECK_EQUAL(token.getValue(), ",");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::IntLiteral);
    BOOST_CHECK_EQUAL(token.getValue(), "3");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::ClosingParenthesis);
    BOOST_CHECK_EQUAL(token.getValue(), ")");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Semicolon);
    BOOST_CHECK_EQUAL(token.getValue(), ";");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::OpeningBracket);
    BOOST_CHECK_EQUAL(token.getValue(), "{");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Let);
    BOOST_CHECK_EQUAL(token.getValue(), "let");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Identifier);
    BOOST_CHECK_EQUAL(token.getValue(), "y");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::TypeOperator);
    BOOST_CHECK_EQUAL(token.getValue(), ":");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::IntTypename);
    BOOST_CHECK_EQUAL(token.getValue(), "int");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::AssignmentOperator);
    BOOST_CHECK_EQUAL(token.getValue(), "=");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::IntLiteral);
    BOOST_CHECK_EQUAL(token.getValue(), "2");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Semicolon);
    BOOST_CHECK_EQUAL(token.getValue(), ";");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Print);
    BOOST_CHECK_EQUAL(token.getValue(), "print");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Identifier);
    BOOST_CHECK_EQUAL(token.getValue(), "calc");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::OpeningParenthesis);
    BOOST_CHECK_EQUAL(token.getValue(), "(");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Identifier);
    BOOST_CHECK_EQUAL(token.getValue(), "x");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Comma);
    BOOST_CHECK_EQUAL(token.getValue(), ",");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Identifier);
    BOOST_CHECK_EQUAL(token.getValue(), "y");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::ClosingParenthesis);
    BOOST_CHECK_EQUAL(token.getValue(), ")");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Semicolon);
    BOOST_CHECK_EQUAL(token.getValue(), ";");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::ClosingBracket);
    BOOST_CHECK_EQUAL(token.getValue(), "}");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Identifier);
    BOOST_CHECK_EQUAL(token.getValue(), "printer");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::OpeningParenthesis);
    BOOST_CHECK_EQUAL(token.getValue(), "(");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::ClosingParenthesis);
    BOOST_CHECK_EQUAL(token.getValue(), ")");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Semicolon);
    BOOST_CHECK_EQUAL(token.getValue(), ";");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Return);
    BOOST_CHECK_EQUAL(token.getValue(), "return");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::IntLiteral);
    BOOST_CHECK_EQUAL(token.getValue(), "0");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Semicolon);
    BOOST_CHECK_EQUAL(token.getValue(), ";");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::ClosingBracket);
    BOOST_CHECK_EQUAL(token.getValue(), "}");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Eof);
    BOOST_CHECK_EQUAL(token.getValue(), "");
  }
  // ^^^^^^ MAIN FUNCTION BODY ^^^^^^

  file.close();
}

BOOST_AUTO_TEST_SUITE_END()