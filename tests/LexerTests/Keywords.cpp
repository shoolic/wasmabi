#include "Lexer/Lexer.hpp"
#include "SourceController/SourceController.hpp"
#include "Token/Token.hpp"
#include "helpers/NullOstream.hpp"
#include <boost/test/unit_test.hpp>
#include <sstream>
#include <string>
using namespace wasmabi;

BOOST_AUTO_TEST_SUITE(LexerKeywordsTest)

OSTREAM_DEFINITION

BOOST_AUTO_TEST_CASE(Select) {
  std::stringstream ss{"select"};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Token token = lexer.nextToken();
  BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), ss.str());
  BOOST_CHECK_EQUAL(token.getType(), Token::Type::Select);
}
BOOST_AUTO_TEST_CASE(Let) {
  std::stringstream ss{"let"};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Token token = lexer.nextToken();
  BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), ss.str());
  BOOST_CHECK_EQUAL(token.getType(), Token::Type::Let);
}
BOOST_AUTO_TEST_CASE(If) {
  std::stringstream ss{"if"};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Token token = lexer.nextToken();
  BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), ss.str());
  BOOST_CHECK_EQUAL(token.getType(), Token::Type::If);
}
BOOST_AUTO_TEST_CASE(Loop) {
  std::stringstream ss{"loop"};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Token token = lexer.nextToken();
  BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), ss.str());
  BOOST_CHECK_EQUAL(token.getType(), Token::Type::Loop);
}
BOOST_AUTO_TEST_CASE(Print) {
  std::stringstream ss{"print"};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Token token = lexer.nextToken();
  BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), ss.str());
  BOOST_CHECK_EQUAL(token.getType(), Token::Type::Print);
}
BOOST_AUTO_TEST_CASE(Fn) {
  std::stringstream ss{"fn"};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Token token = lexer.nextToken();
  BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), ss.str());
  BOOST_CHECK_EQUAL(token.getType(), Token::Type::Fn);
}
BOOST_AUTO_TEST_CASE(Return) {
  std::stringstream ss{"return"};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Token token = lexer.nextToken();
  BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), ss.str());
  BOOST_CHECK_EQUAL(token.getType(), Token::Type::Return);
}
BOOST_AUTO_TEST_CASE(When) {
  std::stringstream ss{"when"};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Token token = lexer.nextToken();
  BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), ss.str());
  BOOST_CHECK_EQUAL(token.getType(), Token::Type::When);
}
BOOST_AUTO_TEST_CASE(Otherwise) {
  std::stringstream ss{"otherwise"};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Token token = lexer.nextToken();
  BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), ss.str());
  BOOST_CHECK_EQUAL(token.getType(), Token::Type::Otherwise);
}
BOOST_AUTO_TEST_CASE(And) {
  std::stringstream ss{"and"};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Token token = lexer.nextToken();
  BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), ss.str());
  BOOST_CHECK_EQUAL(token.getType(), Token::Type::AndOperator);
}
BOOST_AUTO_TEST_CASE(Or) {
  std::stringstream ss{"or"};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Token token = lexer.nextToken();
  BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), ss.str());
  BOOST_CHECK_EQUAL(token.getType(), Token::Type::OrOperator);
}
BOOST_AUTO_TEST_CASE(Not) {
  std::stringstream ss{"not"};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Token token = lexer.nextToken();
  BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), ss.str());
  BOOST_CHECK_EQUAL(token.getType(), Token::Type::NotOperator);
}
BOOST_AUTO_TEST_CASE(Int) {
  std::stringstream ss{"int"};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Token token = lexer.nextToken();
  BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), ss.str());
  BOOST_CHECK_EQUAL(token.getType(), Token::Type::IntTypename);
}
BOOST_AUTO_TEST_CASE(Float) {
  std::stringstream ss{"float"};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Token token = lexer.nextToken();
  BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), ss.str());
  BOOST_CHECK_EQUAL(token.getType(), Token::Type::FloatTypename);
}
BOOST_AUTO_TEST_CASE(String) {
  std::stringstream ss{"string"};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Token token = lexer.nextToken();
  BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), ss.str());
  BOOST_CHECK_EQUAL(token.getType(), Token::Type::StringTypename);
}
BOOST_AUTO_TEST_CASE(Void) {
  std::stringstream ss{"void"};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Token token = lexer.nextToken();
  BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), ss.str());
  BOOST_CHECK_EQUAL(token.getType(), Token::Type::VoidTypename);
}

BOOST_AUTO_TEST_SUITE_END()