cmake_minimum_required(VERSION 3.20)
project(todo CXX)

# ponytail: this MSYS2 install cannot link libstdc++.dll (ld exit 116), so link
# it statically. The call must come before every target that it applies to.
# Remove this block once the toolchain is fixed.
if(MINGW)
    add_link_options(-static-libstdc++ -static-libgcc)
endif()

add_library(todo_core src/todo_core.cpp)
target_include_directories(todo_core PUBLIC src)

add_executable(todo src/todo.cpp)
target_link_libraries(todo PRIVATE todo_core)

enable_testing()
add_subdirectory(tests)
