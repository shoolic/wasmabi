#include "Errors/ErrorHandler.hpp"
#include "Lexer/Lexer.hpp"
#include "ParenthesisPrintVisitor/ParenthesisPrintVisitor.hpp"
#include "Parser/Parser.hpp"
#include "PrintVisitor/PrintVisitor.hpp"
#include "SourceController/SourceController.hpp"
#include "Token/Token.hpp"
#include "helpers/VariantOstream.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>

using namespace wasmabi;

int main(int argc, char *argv[]) {

  if (argc != 2) {
    exit(1);
  }
  std::ifstream file;
  file.open(argv[1]);

  SourceController sourceController(file, std::filesystem::absolute(argv[1]));
  ErrorHandler errorHandler(sourceController, std::cout);
  Lexer lexer(sourceController, errorHandler);
  Parser parser(lexer, errorHandler, std::cout);

  auto ast = parser.parse();

  std::cout << "AST:" << std::endl;
  PrintVisitor printer;
  ParenthesisPrintVisitor parenthesisPrinter;

  printer.visit(*ast);

  return 0;
}