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
  void registerLexicalError(LexicalError &error);

  void registerSyntaxError(SyntaxError &e);
  void registerUnexpectedTokenSyntaxError(UnexpectedToken &e);

private:
  SourceController &sourceController;
  std::ostream &ostream;

  std::ostream &pre();
};

} // namespace wasmabi

#endif // WASMABI_ERRORHANDLER_HPP
