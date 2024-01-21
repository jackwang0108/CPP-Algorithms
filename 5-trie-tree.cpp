#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

using std::cout;
using std::string;
using std::unique_ptr;
using std::unordered_map;
using std::vector;

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
			insertWord(w);
	}

	/**
	 * @brief 向前缀树中加入一个字符串
	 *
	 * @param word 要加入的字符串
	 */
	void insertWord(string &word) const {
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

	/**
     * @brief 从前缀树种删除一个字符串
     * 
     * @param word 要删除的字符串
     */
	void deleteWord(const string &word) const {
		if (word.empty() || search(word) <= 0)
			return;

		TireTreeNode *curr = root.get();
		for (int i = 0; i < word.size(); i++) {
			// pass = 0, 则前缀树中这个字符串已经没有了, 所以释放掉最后一个node
			if (--curr->nexts[word[i]]->pass == 0) {
				curr->nexts.erase(word[i]);
				return;
			}
			curr = curr->nexts[word[i]].get();
		}
		curr->end--;
	}

	/**
	 * @brief 查询字符串word加入了几次
	 *
	 * @param word 要查询的字符串
	 * @return int 字符串加入的数量
	 */
	int search(const string &word) const {
		if (word.empty())
			return 0;

		TireTreeNode *curr = root.get();
		for (int i = 0; i < word.size(); i++) {
			if (!curr->nexts.contains(word[i]))
				return 0;
			curr = curr->nexts[word[i]].get();
		}
		return curr->end;
	}

	/**
     * @brief 返回prefix作为前缀的字符串数量
     * 
     * @param prefix 前缀 
     * @return int 以prefix作为前缀的字符串数量
     */
	int prefix(const string &prefix) const {
		if (prefix.empty())
			return 0;

		TireTreeNode *curr = root.get();
		for (int i = 0; i < prefix.size(); i++) {
			if (!curr->nexts.contains(prefix[i]))
				return 0;
			curr = curr->nexts[prefix[i]].get();
		}
		return curr->pass;
	}
};

int main(int argc, char *argv[]) {
	TireTree tree(vector<string>{"ab", "abc", "bc", "bck", "abe", "abe"});
	cout << "Search ab: " << tree.search(string("ab")) << "\n";
	cout << "Prefix ab: " << tree.prefix(string("ab")) << "\n";
	cout << "Before delete, abe: " << tree.search(string("abe")) << "\n";
	tree.deleteWord(string("abe"));
	cout << "After delete 1, abe: " << tree.search(string("abe")) << "\n";
	tree.deleteWord(string("abe"));
	cout << "After delete 2, abe: " << tree.search(string("abe")) << "\n";


	return 0;
}
