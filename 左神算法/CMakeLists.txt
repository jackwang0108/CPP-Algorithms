cmake_minimum_required(VERSION 3.20)

project(
    左神算法
    VERSION 1.0.0
)

set(CMAKE_CXX_STANDARD 20)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_executable(
    1-sorts-xor-binarysearch-Problems
    "1-sorts-xor-binarysearch-Problems.cpp"
)

add_executable(
    2-linkedlist
    "2-linkedlist.cpp"
)

add_executable(
    3-tree
    "3-tree.cpp"
)

add_executable(
    4-graph
    "4-graph.cpp"
)

add_executable(
    5-trie-tree
    "5-trie-tree.cpp"
)