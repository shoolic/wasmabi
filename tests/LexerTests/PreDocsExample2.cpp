#include "Lexer/Lexer.hpp"
#include "SourceController/SourceController.hpp"
#include "Token/Token.hpp"
#include "helpers/NullOstream.hpp"
#include <boost/test/unit_test.hpp>

#include <fstream>
#include <string>

using namespace wasmabi;

BOOST_AUTO_TEST_SUITE(PreDocsExample2)

OSTREAM_DEFINITION

BOOST_AUTO_TEST_CASE(PreDocsExample2) {
  std::string path("tests/examples/preDocs2.wa");
  std::ifstream file;
  file.open(path);

  SourceController sourceController(file,
                                    std::filesystem::absolute(path).string());
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Let);
    BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "let");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Identifier);
    BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "x");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::TypeOperator);
    BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), ":");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::StringTypename);
    BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "string");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::AssignmentOperator);
    BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "=");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Select);
    BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "select");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::OpeningBracket);
    BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "{");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::StringLiteral);
    BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "Sukces");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::When);
    BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "when");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Identifier);
    BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "score");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::GreaterOrEqual);
    BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), ">=");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::IntLiteral);
    BOOST_CHECK_EQUAL(std::get<int>(token.getValue()), 75);
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Comma);
    BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), ",");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::StringLiteral);
    BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "Moze byc");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::When);
    BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "when");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Identifier);
    BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "score");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::GreaterOrEqual);
    BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), ">=");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::IntLiteral);
    BOOST_CHECK_EQUAL(std::get<int>(token.getValue()), 50);
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Comma);
    BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), ",");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::StringLiteral);
    BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "Porazka");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Otherwise);
    BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "otherwise");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::ClosingBracket);
    BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "}");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Eof);
    BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "");
  }
  file.close();
}

BOOST_AUTO_TEST_SUITE_END()