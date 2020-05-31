# wasmabi
Wasmabi is a simple language and a compiler to LLVM IR created for Compilers classes at Warsaw University of Technology. It also has a CLI built-in support for compiling LLVM IR `.ll` to `.wasm` and to a native binary `.out`.

## Requirements
- `Boost 1.70+`
- `LLVM 10+`
- `clang 10+`
- `wasi-sdk`

## Usage
Type `./bin/Release/wasmabi --help` to see available commands.

### Compilation

## Release
```
./create_configs.sh;
cd build/Release;
make;
cd ../..;
./bin/Release/wasmabi tests/examples/preDocs1.wa;
```

## Debug
```
./create_configs.sh;
cd build/Debug;
make;
cd ../..;
./bin/Debug/wasmabi tests/examples/preDocs1.wa;
```

### Tests
## Run tests
```
./create_configs.sh;
./run_tests.sh;
```

## Enable printing during tests
In `tests/helpers/NullOstream.hpp` there is defined `PREVENT_PRINTING` name. Undefine it to make the program output visible. 
