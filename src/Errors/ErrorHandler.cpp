#include "ErrorHandler.hpp"
#include "../helpers/OstreamModifier.hpp"
#include "../helpers/VariantOstream.hpp"
#include <iostream>

namespace wasmabi {
ErrorHandler::ErrorHandler(SourceController &sourceController_,
                           std::ostream &ostream_)
    : sourceController(sourceController_), ostream(ostream_) {}

void ErrorHandler::registerLexicalError(Error::Lexical error) {
  ostream << OstreamModifier::RED << OstreamModifier::BOLD
          << "Lexical Error:" << std::endl
          << OstreamModifier::END << OstreamModifier::BOLD
          << sourceController.getSourcePath() << ":"
          << sourceController.getCurrentLineNumber() << ":"
          << sourceController.getCurrentColumnNumber() << std::endl
          << OstreamModifier::END << sourceController.getCurrentLine()
          << std::endl
          << std::string(sourceController.getCurrentColumnNumber() - 1, ' ')
          << "^" << std::endl
          << error << std::endl
          << std::endl;
}

void ErrorHandler::registerSyntaxError(SyntaxError *e) {
  ostream << OstreamModifier::RED << OstreamModifier::BOLD
          << "Syntax Error:" << std::endl
          << OstreamModifier::END << OstreamModifier::BOLD
          << sourceController.getSourcePath() << ":"
          << sourceController.getCurrentLineNumber() << ":"
          << sourceController.getCurrentColumnNumber() << std::endl
          << OstreamModifier::END << sourceController.getCurrentLine()
          << std::endl
          << std::string(sourceController.getCurrentColumnNumber() - 1, ' ')
          << "^" << std::endl
          << "Token with type " << e->got.getType() << " and value "
          << e->got.getValue() << " caused error:" << std::endl
          << OstreamModifier::END << e->what();
}

} // namespace wasmabi