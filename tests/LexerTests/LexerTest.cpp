#include "Lexer/Lexer.hpp"
#include "SourceController/SourceController.hpp"
#include "Token/Token.hpp"
#include "helpers/NullOstream.hpp"
#include "helpers/VariantOstream.hpp"

#include <boost/test/unit_test.hpp>
#include <sstream>
#include <string>
using namespace wasmabi;

BOOST_AUTO_TEST_SUITE(LexerTest)

OSTREAM_DEFINITION

BOOST_AUTO_TEST_CASE(NullStream) {
  std::stringstream ss{""};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Token token = lexer.nextToken();
  BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "");
  BOOST_CHECK_EQUAL(token.getType(), Token::Type::Eof);
}

BOOST_AUTO_TEST_CASE(UnknownChar) {
  std::stringstream ss{"@test@$"};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Token token = lexer.nextToken();
  BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "");
  BOOST_CHECK_EQUAL(token.getType(), Token::Type::Invalid);
}

BOOST_AUTO_TEST_CASE(CommentSkip) {
  std::stringstream ss{"@test@"};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Token token = lexer.nextToken();
  BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "");
  BOOST_CHECK_EQUAL(token.getType(), Token::Type::Eof);
}

BOOST_AUTO_TEST_CASE(CommentSkipNoEnd) {
  std::stringstream ss{"@test"};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Token token = lexer.nextToken();
  BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "");
  BOOST_CHECK_EQUAL(token.getType(), Token::Type::Invalid);
}

BOOST_AUTO_TEST_CASE(StringLiteral) {
  std::stringstream ss{"\"This is a string literal\""};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Token token = lexer.nextToken();
  BOOST_CHECK_EQUAL(token.getType(), Token::Type::StringLiteral);
  BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()),
                    "This is a string literal");
}

BOOST_AUTO_TEST_CASE(StringLiteralNoEnd) {
  std::stringstream ss{"\"This is a string literal"};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Token token = lexer.nextToken();
  BOOST_CHECK_EQUAL(token.getType(), Token::Type::Invalid);
  BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()),
                    "This is a string literal");
}

BOOST_AUTO_TEST_CASE(IntLiteral) {
  std::stringstream ss{"12123"};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Token token = lexer.nextToken();
  BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "12123");
  BOOST_CHECK_EQUAL(token.getType(), Token::Type::IntLiteral);
}

BOOST_AUTO_TEST_CASE(FloatLiteral) {
  std::stringstream ss{"102.01023"};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Token token = lexer.nextToken();
  BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "102.01023");
  BOOST_CHECK_EQUAL(token.getType(), Token::Type::FloatLiteral);
}

BOOST_AUTO_TEST_CASE(IntUnexpectedZero) {
  std::stringstream ss{"012"};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Token token = lexer.nextToken();
  BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "0");
  BOOST_CHECK_EQUAL(token.getType(), Token::Type::Invalid);
}

BOOST_AUTO_TEST_CASE(FloatExpectedDigitAfterDot) {
  std::stringstream ss{"1."};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Token token = lexer.nextToken();
  BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "1.");
  BOOST_CHECK_EQUAL(token.getType(), Token::Type::Invalid);
}

BOOST_AUTO_TEST_CASE(FloatTwoDots) {
  std::stringstream ss{"1.2.2"};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Token token = lexer.nextToken();
  BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "1.2");
  BOOST_CHECK_EQUAL(token.getType(), Token::Type::Invalid);
}

BOOST_AUTO_TEST_CASE(CommentSkipWithFollowingToken) {
  std::stringstream ss{"@test@1.2"};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Token token = lexer.nextToken();
  BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "1.2");
  BOOST_CHECK_EQUAL(token.getType(), Token::Type::FloatLiteral);
}

BOOST_AUTO_TEST_CASE(OpeningParenthesis) {
  std::stringstream ss{"("};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Token token = lexer.nextToken();
  BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), ss.str());
  BOOST_CHECK_EQUAL(token.getType(), Token::Type::OpeningParenthesis);
}

BOOST_AUTO_TEST_CASE(ClosingParenthesis) {
  std::stringstream ss{")"};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Token token = lexer.nextToken();
  BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), ss.str());
  BOOST_CHECK_EQUAL(token.getType(), Token::Type::ClosingParenthesis);
}

BOOST_AUTO_TEST_CASE(OpeningBracket) {
  std::stringstream ss{"{"};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Token token = lexer.nextToken();
  BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), ss.str());
  BOOST_CHECK_EQUAL(token.getType(), Token::Type::OpeningBracket);
}

BOOST_AUTO_TEST_CASE(ClosingBracket) {
  std::stringstream ss{"}"};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Token token = lexer.nextToken();
  BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), ss.str());
  BOOST_CHECK_EQUAL(token.getType(), Token::Type::ClosingBracket);
}

BOOST_AUTO_TEST_CASE(AddOperator) {
  std::stringstream ss{"+"};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Token token = lexer.nextToken();
  BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), ss.str());
  BOOST_CHECK_EQUAL(token.getType(), Token::Type::AddOperator);
}

BOOST_AUTO_TEST_CASE(SubOperator) {
  std::stringstream ss{"-"};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Token token = lexer.nextToken();
  BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), ss.str());
  BOOST_CHECK_EQUAL(token.getType(), Token::Type::SubOperator);
}

BOOST_AUTO_TEST_CASE(MulOperator) {
  std::stringstream ss{"*"};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Token token = lexer.nextToken();
  BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), ss.str());
  BOOST_CHECK_EQUAL(token.getType(), Token::Type::MulOperator);
}

BOOST_AUTO_TEST_CASE(DivOperator) {
  std::stringstream ss{"/"};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Token token = lexer.nextToken();
  BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), ss.str());
  BOOST_CHECK_EQUAL(token.getType(), Token::Type::DivOperator);
}

BOOST_AUTO_TEST_CASE(PowOperator) {
  std::stringstream ss{"^"};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Token token = lexer.nextToken();
  BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), ss.str());
  BOOST_CHECK_EQUAL(token.getType(), Token::Type::PowOperator);
}

BOOST_AUTO_TEST_CASE(TypeOperator) {
  std::stringstream ss{":"};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Token token = lexer.nextToken();
  BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), ss.str());
  BOOST_CHECK_EQUAL(token.getType(), Token::Type::TypeOperator);
}

BOOST_AUTO_TEST_CASE(Comma) {
  std::stringstream ss{","};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Token token = lexer.nextToken();
  BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), ss.str());
  BOOST_CHECK_EQUAL(token.getType(), Token::Type::Comma);
}

BOOST_AUTO_TEST_CASE(Semicolon) {
  std::stringstream ss{";"};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Token token = lexer.nextToken();
  BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), ss.str());
  BOOST_CHECK_EQUAL(token.getType(), Token::Type::Semicolon);
}

BOOST_AUTO_TEST_CASE(IntVariableDefinition) {
  std::stringstream ss{"let x_1: int = 12;"};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Let);
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Identifier);
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::TypeOperator);
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::IntTypename);
    BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "int");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::AssignmentOperator);
    BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "=");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::IntLiteral);
    BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "12");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Semicolon);
    BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), ";");
  }
  {
    Token token = lexer.nextToken();
    BOOST_CHECK_EQUAL(token.getType(), Token::Type::Eof);
    BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "");
  }
}

BOOST_AUTO_TEST_SUITE_END()