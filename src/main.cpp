#include "Errors/ErrorHandler.hpp"
#include "Lexer/Lexer.hpp"
#include "Parser/Parser.hpp"

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
  // std::stringstream stream("fn a(q: int): void {"
  //                          "let x: int = 2;"
  //                          "}");
  // SourceController sourceController(stream, "stringstream");
  ErrorHandler errorHandler(sourceController, std::cout);
  Lexer lexer(sourceController, errorHandler);

  // Token token = lexer.nextToken();

  // while (token.getType() != Token::Type::Eof &&
  //        token.getType() != Token::Type::Invalid) {
  //   std::cout << token.getType() << " " << token.getValue() << std::endl;
  //   token = lexer.nextToken();
  // }

  Parser parser(lexer, errorHandler, std::cout);
  auto a = parser.parse();
  a->print(0);
  std::cout << std::endl;

  return 0;
}