#ifndef WASMABI_ERROR_TYPES_HPP
#define WASMABI_ERROR_TYPES_HPP
#include "ErrorTypes.hpp"
#include <iostream>
#include <map>
#include <string>
namespace wasmabi {

class Error {
public:
  enum class Lexical {
    UnexpectedZero,
    ExtraDot,
    ExpectedDigit,
    InvalidToken,
    NoCommentEnd,
    NoQuoteEnd,
  };

private:
  static std::map<Lexical, std::string> lexicalMap;
  friend std::ostream &operator<<(std::ostream &os, const Error::Lexical &err);
};

} // namespace wasmabi

#endif // WASMABI_ERROR_TYPES_HPP
