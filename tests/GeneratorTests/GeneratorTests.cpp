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

BOOST_AUTO_TEST_SUITE_END()