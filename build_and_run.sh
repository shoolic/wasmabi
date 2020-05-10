#!/bin/bash
work_dir=${PWD}

{
cd ${work_dir}/build/Release;
CXX=g++-8 cmake --build . --target wasmabi
}

# {
#     cd ${work_dir};
#     ./bin/Release/wasmabi;
# }



