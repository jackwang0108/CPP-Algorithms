#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <unordered_map>

using std::cout;
using std::string;
using std::unordered_map;
using std::vector;
using std::unique_ptr;

struct TireTreeNode {
	int end = 0;
	int pass = 0;
	unordered_map<char, std::unique_ptr<TireTreeNode>> nexts{};
};

struct TireTree {
    unique_ptr<TireTreeNode> root;

	TireTree() : root(std::make_unique<TireTreeNode>()) {}

	explicit TireTree(vector<string> &&vec)
		: root(std::make_unique<TireTreeNode>()) {
		for (string &w: vec)
			insert(w);
	}

	void insert(string &word) const {
		if (word.empty())
			return;

        root->pass++;
		TireTreeNode *curr = root.get();
		for (int i = 0; i < word.size(); i++) {
			if (!curr->nexts.contains(word[i]))
				curr->nexts.insert(std::make_pair(word[i], std::make_unique<TireTreeNode>()));
			curr = curr->nexts[word[i]].get();
			curr->pass++;
		}
		curr->end++;
	}
};

int main(int argc, char *argv[]) {
	TireTree tree(vector<string>{"ab", "abc", "bc", "bck"});

	return 0;
}
