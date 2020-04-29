#include "Lexer/Lexer.hpp"
#include "SourceController/SourceController.hpp"
#include "Token/Token.hpp"
#include "helpers/NullOstream.hpp"
#include <boost/test/unit_test.hpp>

#include <fstream>
#include <string>

using namespace wasmabi;

BOOST_AUTO_TEST_SUITE(PreDocsExample3)

OSTREAM_DEFINITION

BOOST_AUTO_TEST_CASE(PreDocsExample3) {
  std::string path("tests/examples/preDocs3.wa");
  std::ifstream file;
  file.open(path);

  SourceController sourceController(file,
                                    std::filesystem::absolute(path).string());
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Loop);
    BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "loop");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Identifier);
    BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "x");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Greater);
    BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), ">");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::IntLiteral);
    BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "0");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::AndOperator);
    BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "and");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Identifier);
    BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "y");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::NotEquals);
    BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "!=");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::IntLiteral);
    BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "4");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::OpeningBracket);
    BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "{");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Print);
    BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "print");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Identifier);
    BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "x");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Semicolon);
    BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), ";");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Identifier);
    BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "x");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::AssignmentOperator);
    BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "=");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Identifier);
    BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "x");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::SubOperator);
    BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "-");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::IntLiteral);
    BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "1");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Semicolon);
    BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), ";");
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