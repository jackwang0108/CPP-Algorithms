#include <iostream>
#include <map>
#include <numeric>
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

struct TreeNode {
	int val = 0;
	shared_ptr<TreeNode> left = nullptr;
	shared_ptr<TreeNode> right = nullptr;

	TreeNode() {}
	~TreeNode() {}

	explicit TreeNode(int val) : val(val) {}
	explicit TreeNode(const vector<int> &vec) {
		if (vec.size() == 0)
			return;

		val = vec[0];
		left = buildTree(vec, 1);
		right = buildTree(vec, 2);
	}

	shared_ptr<TreeNode> buildTree(const vector<int> &vec, int idx) {
		if (idx >= vec.size() || vec[idx] == -1)
			return nullptr;

		int leftChildIdx = 2 * idx + 1;
		int rightChildIdx = 2 * idx + 2;
		shared_ptr<TreeNode> curr = std::make_shared<TreeNode>(vec[idx]);
		curr->left = buildTree(vec, leftChildIdx);
		curr->right = buildTree(vec, rightChildIdx);
		return std::move(curr);
	}
};

class BSTtoDoubleLinkedList {
	// 双向链表节点结构和二叉树节点结构是一样的
	// 如果你把last认为是left，next认为是right的话
	// 给定一个搜索二叉树的头节点head，请转化成一条有序的双向链表，并返回链表的头节点。
public:
	// 就是中序遍历
	static shared_ptr<TreeNode> bSTtoDoubleLinkedList(const vector<int> &vec) {
		shared_ptr<TreeNode> root = std::make_shared<TreeNode>(vec);
		shared_ptr<TreeNode> mid = process(root);
		while (mid->left != nullptr)
			mid = mid->left;
		return mid;
	}

	static shared_ptr<TreeNode> process(shared_ptr<TreeNode> &root) {
		if (root == nullptr)
			return nullptr;

		root->left = process(root->left);
		if (root->left != nullptr)
			root->left->right = root;
		root->right = process(root->right);
		if (root->right != nullptr)
			root->right->left = root;
		return root;
	}
};


class BiggestSubBSTInTree {
	// 找到一棵二叉树中，最大的搜索二叉子树，返回最大搜索二叉子树的节点个数。
public:
	// 问题的关键在于, 二叉树的左右子树不一定都是二叉树
	// 因此递归的时候需要返回三个信息, 一个是当前节点为根的子树是否为搜索二叉树, 一个是当前节点为根的子树的最大值和最小值(帮助上级判断是否为二叉树), 一个是节点个数
	// 这里额外加入一个BST根节点的信息, 从而实现如果你要最大搜索二叉子树的根节点, 我也可以给你

	struct Info {
		int nodeSize = 0;
		bool isBST = false;
		int minVal = std::numeric_limits<int>::max();
		int maxVal = std::numeric_limits<int>::min();
		shared_ptr<TreeNode> maxBSTHead = nullptr;
		Info() {}
		Info(int nodeSize, bool isBST, int minVal, int maxVal, shared_ptr<TreeNode> maxBSTHead) : isBST(isBST), minVal(minVal), maxVal(maxVal), nodeSize(nodeSize), maxBSTHead(maxBSTHead) {}
	};

	static int biggestSubBSTInTree(const vector<int> &vec) {
		shared_ptr<TreeNode> root = std::make_shared<TreeNode>(vec);
		return process(root).nodeSize;
	}

	static Info process(const shared_ptr<TreeNode> &root) {
		if (root == nullptr)
			return Info(0, true, std::numeric_limits<int>::max(), std::numeric_limits<int>::min(), nullptr);

		Info currInfo;
		Info leftInfo = process(root->left);
		Info rightInfo = process(root->right);

		// 最大二叉搜索子树就是自己
		if (leftInfo.isBST && rightInfo.isBST && leftInfo.maxVal <= root->val && root->val <= rightInfo.minVal) {
			currInfo.isBST = true;
			currInfo.maxBSTHead = root;
			currInfo.maxVal = std::max(rightInfo.maxVal, root->val);
			currInfo.minVal = std::min(leftInfo.minVal, root->val);
			currInfo.nodeSize = leftInfo.nodeSize + rightInfo.nodeSize + 1;
		} else {
			// 最大搜索二叉子树来自左孩子或者右孩子
			return leftInfo.nodeSize > rightInfo.nodeSize ? leftInfo : rightInfo;
		}

		return currInfo;
	}
};

int main(int argc, char *argv[]) {
	// 打印目录结构
	cout << "Get Folder Tree:\n";
	GetFolderTree::getFolderTree({"a/b/c", "a/d/e", "b/c/k", "b/c/s", "b/k/f", "bf/kc/e"});

	// 二叉树转链表
	cout << "BST to Double Linked List: ";
	auto result = BSTtoDoubleLinkedList::bSTtoDoubleLinkedList({5, 3, 6, 2, 4, -1, 7});
	while (result != nullptr)
		cout << result->val << " ", result = result->right;
	cout << "\n";

	// 最大搜索二叉子树
	cout << "Biggest Sub BST in Tree: " << BiggestSubBSTInTree::biggestSubBSTInTree({3, 5, 7, 1, 6, 4, 8, -1, 2, -1, -1, 2, 10, -1, 9, -1, -1, -1, -1, -1, -1, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}) << "\n";
	return 0;
}
