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

class SubArrayMaxSum {
	// 为了保证招聘信息的质量问题，公司为每个职位设计了打分系统，打分可以为正数，也可以为负数，正数表示用户认可帖子质量，负数表示用户不认可帖子质量．
	// 打分的分数根据评价用户的等级大小不定，比如可以为-1分，10分，30分，-10分等。
	// 假设数组A记录了一条帖子所有打分记录，现在需要找出帖子曾经得到过最高的分数是多少，用于后续根据最高分数来确认需要对发帖用户做相应的惩罚或奖励．
	// 其中，最高分的定义为：用户所有打分记录中，连续打分数据之和的最大值即认为是帖子曾经获得的最高分。
	// 例如：帖子10001010近期的打分记录为[1,1,-1,-10,11,4,-6,9,20,-10,-2],那么该条帖子曾经到达过的最高分数为11+4+(-6)+9+20=38。请实现一段代码，输入为帖子近期的打分记录，输出为当前帖子得到的最高分数。
public:
	// 其实就是最大子数组的和, 可以用dp求解
	// 但是dp求解的时候, 可变变量的选择很重要, 如果选择的是left和right, 那这道题用dp做就是死路一条, 会很难
	// = 合理的使用dp求解的方式是将前缀和设置为可变变量, 但是这个一般是想不到, 所以这里介绍一种新的解题方法: 假设答案法
	// = 假设答案法的核心就是通过假设答案, 从而推导出答案具备的性质, 从而设计出通过遍历来寻找满足性质的答案
	// 对于这道题, 给定数组: [a, b, c, ..., x]
	//              下标:  0, 1, 2, ..., n
	// 假设答案是从i~j的这一段子数组,即vec[i:j], 并且假设vec[i:j]的累加和为Y
	// 因为数组中可能有多个子数组的和都是Y, 所以这里不妨设vec[i:j]使所有累加和为Y的子数组中最长的这一段
	// 那么这个答案就具备如下的性质:
	// 下标: [0, 1, 2, ...., i-1, i, i+1, ..., j-1, j, j+1, ..., n]
	//  1) 对于从i开始到j之前的所有子数组, sum(vec[i,?]) (?<j) 一定大于等于0
	//  因为如果 sum(vec[i,?]) 小于0的话, 那么最大的值就不是i~j了, 而是?~j了,
	//  2) 对于从任意地方开始, 一直到i之前的子数组, sum(vec[?, i-1]) 一定小于0
	//  因为如果i之前还有一段的和大于0, 那么最大值就是i~j了, 而是?~j, 如果等于0, 那么最长的就不是i~j了, 而是?~j
	// 因此, 我们就可以设计如下的算法: 从左向右计算一个累加和curr, 使用一个变量max保存curr的历史最大值, 如果curr变成0了, 那么就说明curr之前的这一段已经不可能是最大了, 因为后面还有可能, 所以curr直接重置为0, 然后重复上面的流程
	// = 假设答案法的题目都是难题, 这种题就是在已知的dp, 贪心等等没有办法给出很好的解的时候, 就假设答案去探索性质, 需要多刷多见
	static int subArrayMaxSum(const vector<int> &vec) {
		int curr = 0;
		int max = vec[0];
		for (int i = 0; i < vec.size(); i++) {
			curr += vec[i];
			max = std::max(max, curr);
			if (curr <= 0)
				curr = 0;
		}
		return max;
	}
};

class SubMatrixMaxSum {
	// 给定一个整型矩阵，返回子矩阵的最大累计和。
public:
	// 核心思路就是把求子矩阵转换为求子数组, 所以大流程上是行上O(N^2), 然后累加
	// = 事实上, 通常来说, 当看到子矩阵问题的时候, 首先先想想子数组如何解决, 而后仿照我们这里的方式进行行的排列就可以解决子矩阵问题
	static int subMatrixMaxSum(const vector<vector<int>> &matrix) {
		int max = std::numeric_limits<int>::min();
		for (int startRow = 0; startRow < matrix.size(); startRow++) {
			for (int endRow = startRow + 1; endRow < matrix.size(); endRow++) {
				vector<int> result(matrix[0].size(), 0);
				for (int i = startRow; i < endRow; i++) {
					std::transform(result.begin(), result.end(), matrix[i].begin(), result.begin(), std::plus<int>());
					max = std::max(max, SubArrayMaxSum::subArrayMaxSum(result));
				}
			}
		}
		return max;
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

	// 最大子数组和
	cout << "Max Sub Array Sum: " << SubArrayMaxSum::subArrayMaxSum({1, 1, -1, -10, 11, 4, -6, 9, 20, -10, 2}) << "\n";

	// 最大子矩阵累计和
	cout << "Max Sub Matrix Sum: " << SubMatrixMaxSum::subMatrixMaxSum({{-5, 3, 6, 4}, {-7, 9, -5, 3}, {-10, 1, -200, 4}}) << "\n";
	return 0;
}
