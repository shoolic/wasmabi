#ifndef WASMABI_ERRORHANDLER_HPP
#define WASMABI_ERRORHANDLER_HPP

#include "../SourceController/SourceController.hpp"
#include "../Token/Token.hpp"
#include "ErrorTypes.hpp"
#include <iostream>

namespace wasmabi {

class ErrorHandler {
public:
  ErrorHandler(SourceController &sourceController_, std::ostream &ostream);
  void registerLexicalError(Error::Lexical error);
  void registerSyntaxError(SyntaxError *e);

private:
  SourceController &sourceController;
  std::ostream &ostream;
};

} // namespace wasmabi

#endif // WASMABI_ERRORHANDLER_HPP
