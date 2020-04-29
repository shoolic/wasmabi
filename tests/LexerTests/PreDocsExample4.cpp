#include "Lexer/Lexer.hpp"
#include "SourceController/SourceController.hpp"
#include "Token/Token.hpp"
#include "helpers/NullOstream.hpp"
#include <boost/test/unit_test.hpp>

#include <fstream>
#include <string>

using namespace wasmabi;

BOOST_AUTO_TEST_SUITE(PreDocsExample4)

OSTREAM_DEFINITION

BOOST_AUTO_TEST_CASE(PreDocsExample4) {
  std::string path("tests/examples/preDocs4.wa");
  std::ifstream file;
  file.open(path);

  SourceController sourceController(file,
                                    std::filesystem::absolute(path).string());
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::If);
    BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "if");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::NotOperator);
    BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "not");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::OpeningParenthesis);
    BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "(");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Identifier);
    BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "x");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::GreaterOrEqual);
    BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), ">=");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::IntLiteral);
    BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "0");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::OrOperator);
    BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "or");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Identifier);
    BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "y");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Greater);
    BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), ">");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::IntLiteral);
    BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "10");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::ClosingParenthesis);
    BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), ")");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::AndOperator);
    BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "and");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Identifier);
    BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "x");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::NotEquals);
    BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "!=");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::SubOperator);
    BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "-");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::IntLiteral);
    BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "5");
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
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::StringLiteral);
    BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "Zaakceptowano");
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