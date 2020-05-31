#!/bin/bash
work_dir=${PWD}

{
cd ${work_dir}/build/Debug;
CXX=g++-8 cmake --build . --target wasmabi
}
