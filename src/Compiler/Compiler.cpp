#include "Compiler.hpp"
#include <boost/process.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "../Errors/ErrorHandler.hpp"
#include "../Generator/Generator.hpp"
#include "../Lexer/Lexer.hpp"
#include "../ParenthesisPrintVisitor/ParenthesisPrintVisitor.hpp"
#include "../Parser/Parser.hpp"
#include "../PrintVisitor/PrintVisitor.hpp"
#include "../SourceController/SourceController.hpp"

namespace wasmabi {
Compiler::Compiler(std::string filename_, std::string target_,
                   std::string wasiSysrootPath_, bool dumpAST_)
    : filename(filename_), target(target_), wasiSysrootPath(wasiSysrootPath_),
      dumpAST(dumpAST_),
      basename(std::filesystem::path(filename).replace_extension()) {}

void Compiler::compile() {

  if (!std::filesystem::exists(filename)) {
    throw NoSourceFileError();
  }

  std::ifstream inputfile(filename);
  SourceController sourceController(inputfile,
                                    std::filesystem::absolute(filename));
  ErrorHandler errorHandler(sourceController, std::cout);
  Lexer lexer(sourceController, errorHandler);
  Parser parser(lexer, errorHandler);

  auto ast = parser.parse();

  if (dumpAST) {
    std::ofstream astOutfile(basename + ".ast");
    PrintVisitor printer(astOutfile);
    printer.visit(*ast);
  }

  std::ofstream outfile(basename + ".ll");
  Generator generator(outfile, basename, sourceController.getSourcePath());
  generator.gen(*ast);

  if (target == "wasm") {
    compileToWasm();
  } else if (target == "native") {
    compileToNative();
  }
}

void Compiler::compileToNative() {
  boost::process::system(
      boost::process::search_path("clang"),
      boost::process::args({"-o", basename + ".out", basename + ".ll", "-lm"}),
      boost::process::std_out > stdout, boost::process::std_err > stderr,
      boost::process::std_in < stdin);
}

void Compiler::compileToWasm() {
  boost::process::system(
      boost::process::search_path("clang"),
      boost::process::args({"--target=wasm32-unknown-wasi",
                            "--sysroot=" + wasiSysrootPath, "-nostartfiles",
                            "-Wl,--no-entry", "-Wl,--export-all", "-o",
                            basename + ".wasm", basename + ".ll"}),

      boost::process::std_out > stdout, boost::process::std_err > stderr,
      boost::process::std_in < stdin);
}

} // namespace wasmabi