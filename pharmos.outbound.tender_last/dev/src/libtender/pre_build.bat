if not exist gen-cpp\ ..\..\ext\thrift\bin\thrift-0.12.0.exe -strict -r -o . --gen cpp ..\..\ext\csc_core_applications\idl\cscservice.thrift

del gen-cpp\*.skeleton.cpp