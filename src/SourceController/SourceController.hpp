#ifndef WASMABI_SOURCE_CONTROLLER_HPP
#define WASMABI_SOURCE_CONTROLLER_HPP

#include <istream>

namespace wasmabi {

class SourceController {
public:
  SourceController(std::istream sourceStream);
  const char getNextChar();
  const std::size_t getCurrentLineNumber() const;
  const std::size_t getCurrentColumnNumber() const;
  const std::size_t getCurrentCharacterColumn() const;
  const std::string getCurrentLine() const;

private:
};

} // namespace wasmabi

#endif // WASMABI_SOURCE_CONTROLLER_HPP