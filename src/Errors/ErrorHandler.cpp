#include "ErrorHandler.hpp"
#include "../Helpers/OstreamModifier.hpp"
#include "../Helpers/VariantOstream.hpp"
#include <iostream>

namespace wasmabi {
ErrorHandler::ErrorHandler(SourceController &sourceController_,
                           std::ostream &ostream_)
    : sourceController(sourceController_), ostream(ostream_) {}

void ErrorHandler::registerLexicalError(LexicalError &error) {
  pre() << error.what() << std::endl << std::endl;
}

std::ostream &ErrorHandler::pre() {
  return ostream << OstreamModifier::RED << OstreamModifier::BOLD
                 << "Error:" << std::endl
                 << OstreamModifier::END << OstreamModifier::BOLD
                 << sourceController.getSourcePath() << ":"
                 << sourceController.getCurrentLineNumber() << ":"
                 << sourceController.getCurrentColumnNumber() << std::endl
                 << OstreamModifier::END << sourceController.getCurrentLine()
                 << std::endl
                 << std::string(sourceController.getCurrentColumnNumber() - 1,
                                ' ')
                 << "^" << std::endl;
}

void ErrorHandler::registerSyntaxError(SyntaxError &e) {
  pre() << "Token with type " << e.got.getType() << " and value "
        << e.got.getValue() << " caused error:" << std::endl
        << OstreamModifier::BOLD << e.what() << OstreamModifier::END
        << std::endl;
}

void ErrorHandler::registerUnexpectedTokenSyntaxError(UnexpectedToken &e) {
  pre() << "Token with type " << e.got.getType() << " and value "
        << e.got.getValue() << " caused error" << std::endl
        << OstreamModifier::BOLD << "Expected " << e.expected
        << OstreamModifier::END << std::endl;
}

} // namespace wasmabi