#!/bin/bash
work_dir=${PWD}

{
cd ${work_dir}/build/Tests;
CXX=g++-8 cmake --build . --target wasmabiTests
}

{
    cd ${work_dir};
    ./bin/Tests/wasmabiTests;
}