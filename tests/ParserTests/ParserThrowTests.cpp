#include "Lexer/Lexer.hpp"
#include "ParenthesisPrintVisitor/ParenthesisPrintVisitor.hpp"
#include "Parser/Parser.hpp"
#include "SourceController/SourceController.hpp"
#include "Token/Token.hpp"

#include "Helpers/NullOstream.hpp"
#include <boost/test/unit_test.hpp>
#include <sstream>
#include <string>
using namespace wasmabi;

BOOST_AUTO_TEST_SUITE(ParserThrowTets)

OSTREAM_DEFINITION

BOOST_AUTO_TEST_CASE(UnexpectedToken) {
  std::stringstream ss{"fn fun() void {"
                       "  print x;"
                       "}"};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Parser parser(lexer, errorHandler);
  BOOST_CHECK_THROW(parser.parse(), UnexpectedToken);
}

BOOST_AUTO_TEST_CASE(UnknownFunRetType) {
  std::stringstream ss{"fn fun(): test {}"};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Parser parser(lexer, errorHandler);
  BOOST_CHECK_THROW(parser.parse(), UnknownFunRetType);
}

BOOST_AUTO_TEST_CASE(UnknownVarRetType) {
  std::stringstream ss{"fn fun(): void { let x: test; }"};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Parser parser(lexer, errorHandler);
  BOOST_CHECK_THROW(parser.parse(), UnknownVarRetType);
}

BOOST_AUTO_TEST_CASE(StatementError) {
  std::stringstream ss{"fn fun(): void { +; }"};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Parser parser(lexer, errorHandler);
  BOOST_CHECK_THROW(parser.parse(), StatementError);
}

BOOST_AUTO_TEST_CASE(VariableDefinitionStatementError) {
  std::stringstream ss{"fn fun(): void { let x:int +;}"};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Parser parser(lexer, errorHandler);
  BOOST_CHECK_THROW(parser.parse(), VariableDefinitionStatementError);
}

BOOST_AUTO_TEST_CASE(SelectExpressionError) {
  std::stringstream ss{"fn fun(): void {"
                       "return select {"
                       "1 ;"
                       "};"
                       "}"};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Parser parser(lexer, errorHandler);
  BOOST_CHECK_THROW(parser.parse(), SelectExpressionError);
}

BOOST_AUTO_TEST_CASE(StatmentWithIdentifierError) {
  std::stringstream ss{" fn fun() : void { x + ; }"};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Parser parser(lexer, errorHandler);
  BOOST_CHECK_THROW(parser.parse(), StatmentWithIdentifierError);
}

BOOST_AUTO_TEST_CASE(ValueExpressionNudError) {
  std::stringstream ss{"fn fun(): void { x = +;}"};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Parser parser(lexer, errorHandler);
  BOOST_CHECK_THROW(parser.parse(), ValueExpressionNudError);
}

BOOST_AUTO_TEST_CASE(ValueExpressionLedError) {
  std::stringstream ss{"fn fun(): void { return 1+; }"};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Parser parser(lexer, errorHandler);
  BOOST_CHECK_THROW(parser.parse(), ValueExpressionLedError);
}

BOOST_AUTO_TEST_CASE(UnexpectedTokenValExprError) {
  std::stringstream ss{"fn pow(base: int, exponent: int) : int{"
                       "return select{"
                       "}}"

  };
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Parser parser(lexer, errorHandler);
  BOOST_CHECK_THROW(parser.parse(), UnexpectedTokenValExprError);
}

BOOST_AUTO_TEST_SUITE_END()