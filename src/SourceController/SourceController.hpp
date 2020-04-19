#ifndef WASMABI_SOURCE_CONTROLLER_HPP
#define WASMABI_SOURCE_CONTROLLER_HPP

#include <filesystem>
#include <istream>

namespace wasmabi {

class SourceController {
public:
  SourceController(std::istream &sourceStream_);
  SourceController(std::istream &sourceStream, std::string sourcePath_);

  char get();
  char peek();
  bool end();
  const std::string getSourcePath();
  std::size_t getCurrentLineNumber() const;
  std::size_t getCurrentColumnNumber() const;
  std::size_t getCurrentCharacterPosition() const;
  const std::string getCurrentLine() const;
  const std::filesystem::path getSourcePath() const;

private:
  bool isNextLineChar(char c) const;

  std::istream &sourceStream;
  std::string sourcePath;
  std::size_t currentLineNumber;
  std::size_t currentColumnNumber;
  std::size_t lastRetCharPos;
  std::size_t currentLinePosition;
  char lastRetChar;
};

} // namespace wasmabi

#endif // WASMABI_SOURCE_CONTROLLER_HPP