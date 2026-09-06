cmake_minimum_required(VERSION 3.20)
project(hello CXX)

add_executable(hello src/hello.cpp)

# ponytail: this MSYS2 install cannot link libstdc++.dll (ld exit 116), so link
# it statically. Remove this block once the toolchain is fixed.
if(MINGW)
    target_link_options(hello PRIVATE -static-libstdc++ -static-libgcc)
endif()
