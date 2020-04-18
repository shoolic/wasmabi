#!/bin/bash
work_dir=${PWD}

{
cd ${work_dir}/build/Tests;
cmake --build . --target wasmabiTests
ctest -VV
}