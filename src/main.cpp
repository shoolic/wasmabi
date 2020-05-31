#include "Compiler/Compiler.hpp"
#include <boost/process.hpp>
#include <boost/program_options.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>

using namespace wasmabi;

int main(int argc, char *argv[]) {
  std::cout << "Using Boost " << BOOST_VERSION / 100000 << "." // major version
            << BOOST_VERSION / 100 % 1000 << "."               // minor version
            << BOOST_VERSION % 100                             // patch level
            << std::endl;
  boost::program_options::options_description desc{"Options"};

  try {
    desc.add_options()("help,h", "Help screen")

        ("target,t", boost::program_options::value<std::string>(),
         "Specify a compilation target, possible values: wasm, native (input "
         "file "
         "must include a main function)")

            ("dump-ast,d",
             boost::program_options::bool_switch()->default_value(false),
             "Dump AST to yourSource.ast file")

                ("wasi-sysroot,wsr",
                 boost::program_options::value<std::string>()->default_value(
                     "/opt/wasi-sdk/share/wasi-sysroot"),
                 "wasi sysroot path")

                    ("input,i", boost::program_options::value<std::string>(),
                     "input file *.wa");

    boost::program_options::positional_options_description p;
    p.add("input", 1);

    boost::program_options::variables_map vm;

    boost::program_options::store(
        boost::program_options::command_line_parser(argc, argv)
            .options(desc)
            .positional(p)
            .run(),
        vm);
    boost::program_options::notify(vm);

    std::string target;
    std::string filename;

    if (vm.count("help")) {
      std::cout << desc << '\n';
    } else {
      if (vm.count("target")) {
        target = vm["target"].as<std::string>();
        if (target != "wasm" && target != "native") {
          throw boost::program_options::error("Incorrect target specified");
        }
      } else {
        target = "wasm";
      }
      if (vm.count("input")) {
        filename = vm["input"].as<std::string>();
        std::string extension = std::filesystem::path(filename).extension();
        if (extension != ".wa") {
          throw boost::program_options::error(
              "Incorrect input extension, .wa expected");
        }
      } else {
        throw boost::program_options::error("No input file provided");
      }

      Compiler compiler(filename, target, vm["wasi-sysroot"].as<std::string>(),
                        vm["dump-ast"].as<bool>());
      compiler.compile();
    }
  } catch (const boost::program_options::error &ex) {
    std::cerr << ex.what() << '\n';
    std::cout << desc << '\n';
  }

  return 0;
}