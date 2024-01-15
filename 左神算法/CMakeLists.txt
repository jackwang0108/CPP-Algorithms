cmake_minimum_required(VERSION 3.20)

project(
    左神算法
    VERSION 1.0.0
)

set(CMAKE_CXX_STANDARD 20)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_executable(
    1-sorts-xor-binarysearch
    "./1-sorts-xor-binarysearch.cpp"
)
