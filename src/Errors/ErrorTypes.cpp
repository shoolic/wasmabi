#include "ErrorTypes.hpp"

namespace wasmabi {

std::map<Error::Lexical, std::string> Error::lexicalMap = {
    {Error::Lexical::UnexpectedZero,
     "Incorrect number format (unexpected zero at the beginning?)"},
    {Error::Lexical::ExtraDot, "Incorrect number format (extra dot?)"},
    {Error::Lexical::ExpectedDigit, "Incorrect number format (digit expected)"},
    {Error::Lexical::InvalidToken, "Invalid token"},
    {Error::Lexical::NoCommentEnd, "No comment end"},
    {Error::Lexical::NoQuoteEnd, "No quote end"},
};

std::ostream &operator<<(std::ostream &os, const Error::Lexical &err) {
  os << Error::lexicalMap[err];
  return os;
}

} // namespace wasmabi