#!/bin/bash

DIR=$(cd $(dirname $0) && pwd)
#generate cpp from IDL files

cd "$DIR"
rm -rf libtender/gen-cpp/

../ext/thrift/bin/thrift -strict -r -o libtender --gen cpp "$DIR/../ext/csc_core_applications/idl/cscservice.thrift"

rm -rf libtender/gen-cpp/*.skeleton.cpp
