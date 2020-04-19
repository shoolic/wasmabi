#include "Lexer/Lexer.hpp"
#include "SourceController/SourceController.hpp"
#include "Token/Token.hpp"
#include <boost/test/unit_test.hpp>

#include <fstream>
#include <string>

using namespace wasmabi;

BOOST_AUTO_TEST_SUITE(PreDocsExample5)

BOOST_AUTO_TEST_CASE(PreDocsExample5) {
  std::ifstream file;
  file.open("tests/examples/preDocs5.wa");

  SourceController sourceController(file);
  Lexer lexer(sourceController);
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Identifier);
    BOOST_CHECK_EQUAL(token.getValue(), "x");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::AssignmentOperator);
    BOOST_CHECK_EQUAL(token.getValue(), "=");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::OpeningParenthesis);
    BOOST_CHECK_EQUAL(token.getValue(), "(");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::FloatLiteral);
    BOOST_CHECK_EQUAL(token.getValue(), "2.1");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::AddOperator);
    BOOST_CHECK_EQUAL(token.getValue(), "+");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::FloatLiteral);
    BOOST_CHECK_EQUAL(token.getValue(), "3.7");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::ClosingParenthesis);
    BOOST_CHECK_EQUAL(token.getValue(), ")");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::MulOperator);
    BOOST_CHECK_EQUAL(token.getValue(), "*");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::SubOperator);
    BOOST_CHECK_EQUAL(token.getValue(), "-");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::FloatLiteral);
    BOOST_CHECK_EQUAL(token.getValue(), "0.6");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Semicolon);
    BOOST_CHECK_EQUAL(token.getValue(), ";");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Eof);
    BOOST_CHECK_EQUAL(token.getValue(), "");
  }
  file.close();
}

BOOST_AUTO_TEST_SUITE_END()