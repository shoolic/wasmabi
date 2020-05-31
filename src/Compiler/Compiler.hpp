#ifndef WASMABI_COMPILATOR_HPP
#define WASMABI_COMPILATOR_HPP
#include <string>

namespace wasmabi {

class Compiler {

public:
  Compiler(std::string filename_, std::string target_,
           std::string wasiSysrootPath_, bool dumpAST_);
  void compile();

private:
  std::string filename;
  std::string target;
  std::string wasiSysrootPath;
  bool dumpAST;
  std::string basename;

  void compileToWasm();
  void compileToNative();
};

} // namespace wasmabi

#endif // WASMABI_COMPILATOR_HPP
