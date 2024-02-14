#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using std::cout;
using std::map;
using std::shared_ptr;
using std::string;
using std::stringstream;
using std::unordered_map;
using std::vector;

class GetFolderTree {
	// 给你一个字符串类型的数组arr，譬如：arr={"b\\cst","d\\","a\\d\\e","a\\b\\c"};
	// 你把这些路径中蕴含的目录结构给画出来，子目录直接列在父目录下面，并比父目录向右进两格，就像这样:
	// a
	//   b
	//     c
	//   d
	//     e
	// b
	//   cst
	// d
	// 同一级的需要按字母顺序排列，不能乱
public:
	// 使用前缀树, 然后深度优先遍历即可
	struct TrieTreeNode {
		int end = 0;
		int pass = 0;
		map<string, shared_ptr<TrieTreeNode>> nexts{};
	};

	struct TrieTree {
		shared_ptr<TrieTreeNode> root;

		TrieTree() : root(std::make_shared<TrieTreeNode>()) {}

		explicit TrieTree(const vector<vector<string>> &words) : root(std::make_shared<TrieTreeNode>()) {
			for (const vector<string> &word: words)
				insertWord(word);
		}

		void insertWord(const vector<string> &word) {
			if (word.empty())
				return;

			root->pass++;
			shared_ptr<TrieTreeNode> curr = root;
			for (int i = 0; i < word.size(); i++) {
				if (!curr->nexts.contains(word[i]))
					curr->nexts.insert(std::make_pair(word[i], std::make_shared<TrieTreeNode>()));
				curr = curr->nexts[word[i]];
				curr->pass++;
			}
			curr->end++;
		}
	};

	static void getFolderTree(const vector<string> &arr) {
		vector<vector<string>> vecPaths;
		for (const string &path: arr) {
			vector<string> vecPath;
			string pathComponent;
			stringstream ss(path);
			while (std::getline(ss, pathComponent, '/'))
				vecPath.push_back(pathComponent);
			vecPaths.push_back(vecPath);
		}

		TrieTree tree(vecPaths);

		// 深度优先遍历前缀树
		process(tree.root, 0);
	}

	static void process(shared_ptr<TrieTreeNode> curr, int depth) {
		if (curr->nexts.empty())
			return;
		for (const std::pair<string, shared_ptr<TrieTreeNode>> &component: curr->nexts) {
			for (int i = 0; i < 2 * depth; i++)
				cout << " ";
			cout << component.first << "\n";
			process(component.second, depth + 1);
		}
	}
};


int main(int argc, char *argv[]) {
	// 打印目录结构
	GetFolderTree::getFolderTree({"a/b/c", "a/d/e", "b/c/k", "b/c/s", "b/k/f", "bf/kc/e"});
	return 0;
}
