cmake_minimum_required(VERSION 3.20)

project(
	左神算法
	VERSION 2.0.0
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

add_executable(
	6-greedy-algorithm
	"6-greedy-algorithm.cpp"
)

add_executable(
	7-force-recursion
	"7-force-recursion.cpp"
)

add_executable(
    8-hashfunction-and-hashmap
    "8-hashfunction-and-hashmap.cpp"
)

add_executable(
    9-union-fold
    "9-union-fold.cpp"
)

add_executable(
    10-kmp-manacher
    "10-kmp-manacher.cpp"
)

add_executable(
    11-siliding-window-monotone-stack
    "11-siliding-window-monotone-stack.cpp"
)

add_executable(
    12-tree-dp-morris-traversal
    "12-tree-dp-morris-traversal.cpp"
)

add_executable(
    13-big-data
    "13-big-data.cpp"
)

add_executable(
    14-bit-operatoion
    "14-bit-operatoion.cpp"
)

add_executable(
    15-dynamic-programming
    "15-dynamic-programming.cpp"
)

add_executable(
    16-balancedBinaryTree-AVLTree-SizeBalancedTree-SkipList
    "16-balancedBinaryTree-AVLTree-SizeBalancedTree-SkipList.cpp"
)