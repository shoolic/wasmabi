# wasmabi
Wasmabi is a simple language and a compiler to LLVM IR created for Compilers classes at Warsaw University of Technology. 


# Level of development
Program prints list of token type and token value (lexer implementation).

# Release
```
./create_configs.sh;
cd build/Release;
make;
cd ../..;
./bin/Release/wasmabi tests/examples/preDocs1.wa;
```

# Debug
```
./create_configs.sh;
cd build/Debug;
make;
cd ../..;
./bin/Debug/wasmabi tests/examples/preDocs1.wa;
```

# Run tests
```
./create_configs.sh;
./run_tests.sh;
```

# Enable printing during tests
In `tests/helpers/NullOstream.hpp` there is defined `PREVENT_PRINTING` name. Undefine it to make the program output visible. 