#!/bin/bash
work_dir=${PWD}

rm -R ${work_dir}/build

mkdir ${work_dir}/build
mkdir ${work_dir}/build/Release
mkdir ${work_dir}/build/Debug

{
cd ${work_dir}/build/Release;
cmake ../../ -DCMAKE_BUILD_TYPE="Release"
}

{
cd ${work_dir}/build/Debug;
cmake ../../ -DCMAKE_BUILD_TYPE="Debug"
}
