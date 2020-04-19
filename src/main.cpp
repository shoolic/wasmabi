#include "Lexer/Lexer.hpp"
#include "SourceController/SourceController.hpp"
#include "Token/Token.hpp"
#include <fstream>
#include <iostream>

using namespace wasmabi;

int main(int argc, char *argv[]) {

  if (argc != 2) {
    exit(1);
  }
  std::ifstream file;
  file.open(argv[1]);

  SourceController sourceController(file);
  Lexer lexer(sourceController);

  Token token = lexer.nextToken();

  while (token.getType() != Token::Type::Eof) {
    std::cout << token.getType() << " " << token.getValue() << std::endl;
    token = lexer.nextToken();
  }

  return 0;
}