#include "Generator/Generator.hpp"
#include "Lexer/Lexer.hpp"
#include "ParenthesisPrintVisitor/ParenthesisPrintVisitor.hpp"
#include "Parser/Parser.hpp"
#include "SourceController/SourceController.hpp"
#include "Token/Token.hpp"

#include "Helpers/NullOstream.hpp"
#include <boost/test/unit_test.hpp>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
using namespace wasmabi;

BOOST_AUTO_TEST_SUITE(GeneratorTests)

OSTREAM_DEFINITION

BOOST_AUTO_TEST_CASE(GeneratorPreDocsExampleTest) {
  std::string path("tests/examples/preDocs1.wa");
  std::ifstream file;
  file.open(path);

  SourceController sourceController(file,
                                    std::filesystem::absolute(path).string());

  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Parser parser(lexer, errorHandler);
  auto ast = parser.parse();

  std::stringstream out{};

  Generator generator(out, "sourceStream", "sourceStream");
  generator.gen(*ast);

  std::ifstream compareIRFile("tests/examples-ll/preDocs1.ll");
  std::stringstream compareIR;
  compareIR << compareIRFile.rdbuf();

  BOOST_CHECK_EQUAL(out.str(), compareIR.str());
}

BOOST_AUTO_TEST_CASE(UndefinedFunctionError) {
  std::stringstream ss{"fn fun(): void { undef(); }"};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Parser parser(lexer, errorHandler);
  auto ast = parser.parse();
  std::stringstream out{};
  Generator generator(out, "sourceStream", "sourceStream");

  BOOST_CHECK_THROW(generator.gen(*ast), wasmabi::UndefinedFunctionError);
}

BOOST_AUTO_TEST_CASE(StringAsOperandError) {
  std::stringstream ss{"fn fun(): void { let x: string = -\"ff\"; }"};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Parser parser(lexer, errorHandler);
  auto ast = parser.parse();
  std::stringstream out{};
  Generator generator(out, "sourceStream", "sourceStream");
  BOOST_CHECK_THROW(generator.gen(*ast), wasmabi::StringAsOperandError);
}

BOOST_AUTO_TEST_CASE(StringAsOperandErrorBinary) {
  std::stringstream ss{"fn fun(): void { let x: string = 1+\"ff\"; }"};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Parser parser(lexer, errorHandler);
  auto ast = parser.parse();
  std::stringstream out{};
  Generator generator(out, "sourceStream", "sourceStream");
  BOOST_CHECK_THROW(generator.gen(*ast), wasmabi::StringAsOperandError);
}

BOOST_AUTO_TEST_CASE(FunctionReturnValueMismatchError) {
  std::stringstream ss{"fn fun(): int { return 0.0; }"};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Parser parser(lexer, errorHandler);
  auto ast = parser.parse();
  std::stringstream out{};
  Generator generator(out, "sourceStream", "sourceStream");
  BOOST_CHECK_THROW(generator.gen(*ast),
                    wasmabi::FunctionReturnValueMismatchError);
}

BOOST_AUTO_TEST_CASE(VoidFunretValueError) {
  std::stringstream ss{"fn fun(): void { return; }"};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Parser parser(lexer, errorHandler);
  auto ast = parser.parse();
  std::stringstream out{};
  Generator generator(out, "sourceStream", "sourceStream");
  BOOST_CHECK_THROW(generator.gen(*ast), wasmabi::VoidFunretValueError);
}

BOOST_AUTO_TEST_CASE(VarAssignTypeMismatch) {
  std::stringstream ss{"fn fun(): void { let x: int = 0.0; }"};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Parser parser(lexer, errorHandler);
  auto ast = parser.parse();
  std::stringstream out{};
  Generator generator(out, "sourceStream", "sourceStream");
  BOOST_CHECK_THROW(generator.gen(*ast), wasmabi::VarAssignTypeMismatch);
}

BOOST_AUTO_TEST_CASE(UndefinedVariableError) {
  std::stringstream ss{"fn fun(): void { x = 0.0; }"};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Parser parser(lexer, errorHandler);
  auto ast = parser.parse();
  std::stringstream out{};
  Generator generator(out, "sourceStream", "sourceStream");
  BOOST_CHECK_THROW(generator.gen(*ast), wasmabi::UndefinedVariableError);
}

BOOST_AUTO_TEST_CASE(VariableRedefintionError) {
  std::stringstream ss{"fn fun(): void { let x: int; let x: int; }"};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Parser parser(lexer, errorHandler);
  auto ast = parser.parse();
  std::stringstream out{};
  Generator generator(out, "sourceStream", "sourceStream");
  BOOST_CHECK_THROW(generator.gen(*ast), wasmabi::VariableRedefintionError);
}

BOOST_AUTO_TEST_CASE(StringAsConditionError) {
  std::stringstream ss{"fn fun(): void { if \"X\" {} }"};
  SourceController sourceController(ss);
  ErrorHandler errorHandler(sourceController, TEST_OSTREAM);
  Lexer lexer(sourceController, errorHandler);
  Parser parser(lexer, errorHandler);
  auto ast = parser.parse();
  std::stringstream out{};
  Generator generator(out, "sourceStream", "sourceStream");
  BOOST_CHECK_THROW(generator.gen(*ast), wasmabi::StringAsConditionError);
}

BOOST_AUTO_TEST_SUITE_END()