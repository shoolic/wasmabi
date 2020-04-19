#include "Errors/ErrorHandler.hpp"
#include "Lexer/Lexer.hpp"
#include "SourceController/SourceController.hpp"
#include "Token/Token.hpp"
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

  Token token = lexer.nextToken();

  while (token.getType() != Token::Type::Eof &&
         token.getType() != Token::Type::Invalid) {
    std::cout << token.getType() << " " << token.getValue() << std::endl;
    token = lexer.nextToken();
  }

  return 0;
}