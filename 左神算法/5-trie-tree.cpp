#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>

using std::cout;
using std::string;
using std::unordered_map;
using std::vector;

struct TireTreeNode {
	int end = 0;
	int pass = 0;
	unordered_map<char, TireTreeNode *> nexts{};
};

struct TireTree {
	TireTreeNode *root{nullptr};

	TireTree() : root(new TireTreeNode) {}

	explicit TireTree(vector<string> &&vec)
		: root(new TireTreeNode) {
		for (string &w: vec)
			insert(w);
	}

	void insert(string &word) const {
		if (word.empty())
			return;

		root->pass++;
		TireTreeNode *curr = root;
		for (int i = 0; i < word.size(); i++) {
			if (!curr->nexts.contains(word[i]))
				curr->nexts.insert(std::make_pair(word[i], new TireTreeNode));
			curr = curr->nexts[word[i]];
			curr->pass++;
		}
		curr->end++;
	}
};

int main(int argc, char *argv[]) {
	TireTree tree(vector<string>{"ab", "abc", "bc", "bck"});
	cout << "Test";

	return 0;
}
