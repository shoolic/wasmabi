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

BOOST_AUTO_TEST_SUITE(ParserTets)

OSTREAM_DEFINITION

BOOST_AUTO_TEST_CASE(PowRightAssociattion) {
  std::stringstream ss{"fn c(): void {"
                       "x = 3 ^ 4 ^ 5;"
                       "}"};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Parser parser(lexer, errorHandler);
  auto ast = parser.parse();

  VariableAssignmentStatement *s =
      (VariableAssignmentStatement *)(std::get<std::unique_ptr<Statement>>(
                                          ast->functionDefintions[0]
                                              ->block->instructions[0])
                                          .get());

  ParenthesisPrintVisitor parenthesisPrinter;
  BOOST_CHECK_EQUAL(parenthesisPrinter.getString(*(s->value)),
                    "((3)^((4)^(5)))");
}

BOOST_AUTO_TEST_CASE(Parenthesis) {
  std::stringstream ss{"fn c(): void {"
                       "x = (2+2)*2;"
                       "}"};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Parser parser(lexer, errorHandler);
  auto ast = parser.parse();

  VariableAssignmentStatement *s =
      (VariableAssignmentStatement *)(std::get<std::unique_ptr<Statement>>(
                                          ast->functionDefintions[0]
                                              ->block->instructions[0])
                                          .get());

  ParenthesisPrintVisitor parenthesisPrinter;
  BOOST_CHECK_EQUAL(parenthesisPrinter.getString(*(s->value)),
                    "(((2)+(2))*(2))");
}

BOOST_AUTO_TEST_CASE(UnaryMinus) {
  std::stringstream ss{"fn c(): void {"
                       "x = -2+-2;"
                       "}"};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Parser parser(lexer, errorHandler);
  auto ast = parser.parse();

  VariableAssignmentStatement *s =
      (VariableAssignmentStatement *)(std::get<std::unique_ptr<Statement>>(
                                          ast->functionDefintions[0]
                                              ->block->instructions[0])
                                          .get());

  ParenthesisPrintVisitor parenthesisPrinter;
  BOOST_CHECK_EQUAL(parenthesisPrinter.getString(*(s->value)),
                    "((-(2))+(-(2)))");
}

BOOST_AUTO_TEST_CASE(FunCall) {
  std::stringstream ss{"fn c(): void {"
                       "x = f(2+2,5*5);"
                       "}"};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Parser parser(lexer, errorHandler);
  auto ast = parser.parse();

  VariableAssignmentStatement *s =
      (VariableAssignmentStatement *)(std::get<std::unique_ptr<Statement>>(
                                          ast->functionDefintions[0]
                                              ->block->instructions[0])
                                          .get());

  ParenthesisPrintVisitor parenthesisPrinter;
  BOOST_CHECK_EQUAL(parenthesisPrinter.getString(*(s->value)),
                    "(f(((2)+(2)),((5)*(5))))");
}

BOOST_AUTO_TEST_CASE(OperatorPrecedence) {
  std::stringstream ss{
      "fn c(): void {"
      "x = not x+1-2*4/5^6 or 7 < 8 <= 9 > 10 >= 11 != 12 == 13 and 14;"
      "}"};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Parser parser(lexer, errorHandler);
  auto ast = parser.parse();

  VariableAssignmentStatement *s =
      (VariableAssignmentStatement *)(std::get<std::unique_ptr<Statement>>(
                                          ast->functionDefintions[0]
                                              ->block->instructions[0])
                                          .get());

  ParenthesisPrintVisitor parenthesisPrinter;
  BOOST_CHECK_EQUAL(
      parenthesisPrinter.getString(*(s->value)),
      "((((((not(x))+(1))-(((2)*(4))/"
      "((5)^(6))))or(((((7)<(8))<=(9))>(10))>=(11)))!=((12)==(13)))and(14))");
}

BOOST_AUTO_TEST_SUITE_END()