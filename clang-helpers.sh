#!/bin/bash
# this is a helper file containing clang commands with different options
# export all

clang \
--target=wasm32-unknown-wasi \
--sysroot=/opt/wasi-sdk/share/wasi-sysroot  \
-nostartfiles \
-Wl,--no-entry \
-Wl,--export-all \
-o test.wasm \
test.ll;

# no export 
clang \
--target=wasm32-unknown-wasi \
--sysroot /opt/wasi-sdk/share/wasi-sysroot  \
-o test.wasm \
test.ll

# export main
clang \
--target=wasm32-unknown-wasi \
--sysroot /opt/wasi-sdk/share/wasi-sysroot  \
-o test.wasm \
-Wl,--no-entry \
-Wl,--export=main \
test.ll

# export native

clang \
-lm \
-o test.out \
test.ll


