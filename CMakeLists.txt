cmake_minimum_required(VERSION 3.20)
project(todo CXX)

add_executable(todo src/todo.cpp)

# ponytail: this MSYS2 install cannot link libstdc++.dll (ld exit 116), so link
# it statically. Remove this block once the toolchain is fixed.
if(MINGW)
    target_link_options(todo PRIVATE -static-libstdc++ -static-libgcc)
endif()

enable_testing()
add_subdirectory(tests)
