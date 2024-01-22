cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(
    NewCoder-Programmer-Interview-Guide
    VERSION 0.0.1
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)


add_executable(
    CD1
    "CD1-在行列都排好序的矩阵中找指定的数.cpp"
)