#include "SourceController.hpp"

#include <iostream>

namespace wasmabi {

SourceController::SourceController(std::istream &sourceStream_)
    : SourceController(sourceStream_, "sourceStream") {}

SourceController::SourceController(std::istream &sourceStream_,
                                   std::string sourcePath_)
    : sourceStream(sourceStream_), sourcePath(sourcePath_),
      currentLineNumber(1), currentColumnNumber(1), lastRetCharPos(-1),
      currentLinePosition(0), lastRetChar('\0') {}

char SourceController::get() {
  char currentChar = '\0';
  sourceStream.get(currentChar);

  currentColumnNumber++;
  lastRetCharPos++;

  if (isNextLineChar(currentChar)) {
    if (!(lastRetChar == '\r' && currentChar == '\n')) {
      currentLineNumber++;
    }
    currentLinePosition = lastRetCharPos + 1;
    currentColumnNumber = 0;
  }

  lastRetChar = currentChar;
  return currentChar;
}

char SourceController::peek() { return sourceStream.peek(); }

const std::string SourceController::getSourcePath() { return sourcePath; }

std::size_t SourceController::getCurrentLineNumber() const {
  return currentLineNumber;
}

std::size_t SourceController::getCurrentColumnNumber() const {
  return currentColumnNumber;
}

std::size_t SourceController::getCurrentCharacterPosition() const {
  return lastRetCharPos;
}

const std::string SourceController::getCurrentLine() const {
  sourceStream.seekg(currentLinePosition);

  std::string line;
  std::getline(sourceStream, line);

  sourceStream.seekg(lastRetCharPos);

  return line;
}

bool SourceController::isNextLineChar(char c) const {
  return c == '\n' || c == '\r';
}

bool SourceController::end() { return sourceStream.peek() == EOF; }

} // namespace wasmabi