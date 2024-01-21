#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <utility>
#include <vector>

using std::cout;
using std::endl;
using std::map;
using std::pair;
using std::queue;
using std::set;
using std::stack;
using std::string;
using std::vector;

struct TreeNode {
	int value;
	TreeNode *left;
	TreeNode *right;

	TreeNode() : value(0), left(nullptr), right(nullptr) {}

	explicit TreeNode(int data) : value(data), left(nullptr), right(nullptr) {}

	explicit TreeNode(vector<int> &&vec) : value(0), left(nullptr), right(nullptr) {
		if (vec.empty()) {
			return;
		}
		value = vec[0];
		left = buildTree(vec, 1);
		right = buildTree(vec, 2);
	}

	TreeNode *buildTree(vector<int> &vec, int rootIdx) {
		if (rootIdx >= vec.size() || vec[rootIdx] == -1)
			return nullptr;

		int leftChildIdx = rootIdx * 2 + 1, rightChildIdx = rootIdx * 2 + 2;
		TreeNode *root = new TreeNode;

		root->value = vec[rootIdx];
		root->left = buildTree(vec, leftChildIdx);
		root->right = buildTree(vec, rightChildIdx);
		return root;
	}

	string serial() {
		return _serial(this);
	}

	string _serial(TreeNode *root) {
		if (root == nullptr)
			return "#_";

		string result = std::to_string(root->value) + "_";
		result += _serial(root->left);
		result += _serial(root->right);
		return result;
	}

	static TreeNode *unserial(string &serialedStr) {
		queue<string> que;
		string delimiter = "_";
		size_t pos = 0;
		while ((pos = serialedStr.find(delimiter)) != std::string::npos) {
			que.push(serialedStr.substr(0, pos));
			serialedStr.erase(0, pos + delimiter.length());
		}

		return _unserial(que);
	}

	static TreeNode *_unserial(queue<string> &que) {
		string &valueStr = que.front();
		que.pop();

		if (valueStr == "#")
			return nullptr;

		TreeNode *root = new TreeNode(std::stoi(valueStr));
		root->left = _unserial(que);
		root->right = _unserial(que);
		return root;
	}
};

struct ParentTreeNode {
	int value;
	ParentTreeNode *parent;
	ParentTreeNode *left;
	ParentTreeNode *right;

	ParentTreeNode() : value(0), parent(nullptr), left(nullptr), right(nullptr) {}
	explicit ParentTreeNode(int value) : value(value), parent(nullptr), left(nullptr), right(nullptr) {}

	explicit ParentTreeNode(vector<int> &&vec) : value(0), parent(nullptr), left(nullptr), right(nullptr) {
		if (vec.empty())
			return;

		value = vec[0];
		left = buildTree(vec, 1, this);
		right = buildTree(vec, 2, this);
	}

	ParentTreeNode *buildTree(vector<int> &vec, int rootIdx, ParentTreeNode *parent) {
		if (rootIdx >= vec.size() || vec[rootIdx] == -1)
			return nullptr;
		int leftChildIdx = 2 * rootIdx + 1,
		    rightChildIdx = 2 * rootIdx + 2;
		ParentTreeNode *root = new ParentTreeNode;

		root->value = vec[rootIdx];
		root->parent = parent;
		root->left = buildTree(vec, leftChildIdx, root);
		root->right = buildTree(vec, rightChildIdx, root);
		return root;
	}
};

class TreeDepthFirstTraversal {
public:
	// 先序遍历递归版
	static void preorderRecurr(TreeNode *root) {
		if (root == nullptr)
			return;
		cout << root->value << " ";
		preorderRecurr(root->left);
		preorderRecurr(root->right);
	}

	// 中序遍历递归版
	static void inorderRecurr(TreeNode *root) {
		if (root == nullptr)
			return;
		inorderRecurr(root->left);
		cout << root->value << " ";
		inorderRecurr(root->right);
	}

	// 后序遍历递归版
	static void postorderRecurr(TreeNode *root) {
		if (root == nullptr)
			return;
		postorderRecurr(root->left);
		postorderRecurr(root->right);
		cout << root->value << " ";
	}

	// 先序遍历非递归版
	static void preorderStack(TreeNode *root) {
		stack<TreeNode *> st;
		st.push(root);

		TreeNode *curr;
		while (!st.empty()) {
			curr = st.top(), st.pop();
			if (curr == nullptr)
				continue;
			cout << curr->value << " ";
			st.push(curr->right);
			st.push(curr->left);
		}
	}

	// 中序遍历非递归版
	static void inorderStack(TreeNode *root) {
		// 中序遍历, 先把左孩子进栈, 直到左孩子为空, 然后右孩子进栈, 然后对右孩子重复这个处理
		stack<TreeNode *> st;
		while (!st.empty() || root != nullptr) {
			if (root != nullptr) {
				st.push(root);
				root = root->left;
			} else {
				root = st.top(), st.pop();
				cout << root->value << " ";
				root = root->right;
			}
		}
	}

	// 后序遍历非递归版
	static void postorderStack(TreeNode *root) {
		// 后序遍历输出顺序是左右头, 再加一个收集栈反向之后, 只要打印顺序是头右左即可
		stack<TreeNode *> st, print;
		st.push(root);

		TreeNode *curr;
		while (!st.empty()) {
			curr = st.top(), st.pop();
			if (curr == nullptr)
				continue;
			print.push(curr);
			st.push(curr->left);
			st.push(curr->right);
		}

		while (!print.empty()) {
			curr = print.top(), print.pop();
			cout << curr->value << " ";
		}
	}
};

// 广度优先遍历
class TreeWidthFirstTraversal {
public:
	static void widthFirstTraversal(TreeNode *root) {
		queue<TreeNode *> st;
		st.push(root);

		TreeNode *curr;
		while (!st.empty()) {
			curr = st.front(), st.pop();
			if (curr == nullptr)
				continue;
			cout << curr->value << " ";
			st.push(curr->left);
			st.push(curr->right);
		}
	}
};

class TreeProblems {
public:
	// 求一个树的最大宽度
	static int maxWidth1(TreeNode *root) {
		queue<TreeNode *> q;
		map<TreeNode *, int> levelMap;

		q.push(root);
		levelMap.insert(std::make_pair(root, 1));
		TreeNode *curr;
		int currDepth = 1, currWidth = 0, maxWidth = 0;
		while (!q.empty()) {
			curr = q.front(), q.pop();
			if (curr == nullptr)
				continue;

			int currNodeDepth = levelMap[curr];
			if (currNodeDepth == currDepth)
				currWidth++;
			else {
				currDepth += 1;
				maxWidth = std::max(currWidth, maxWidth);
				currWidth = 1;
			}

			q.push(curr->left);
			levelMap[curr->left] = currNodeDepth + 1;
			q.push(curr->right);
			levelMap[curr->right] = currNodeDepth + 1;
		}

		return maxWidth;
	}

	// 求一个树最大宽度, 不使用哈希表, 使用变量标记
	static int maxWidth2(TreeNode *root) {
		queue<TreeNode *> qu;

		qu.push(root);
		TreeNode *curr, *currEnd = root, *special = new TreeNode;
		qu.push(special);
		int currDepth = 1, currWidth = 0, maxWidth = 0;
		while (qu.size() > 1) {
			curr = qu.front(), qu.pop();

			if (curr->left != nullptr)
				qu.push(curr->left);
			if (curr->right != nullptr)
				qu.push(curr->right);

			// 判断是否为特殊标记的尾部
			if (curr == special) {
				currDepth++;
				maxWidth = std::max(maxWidth, currWidth);
				currWidth = 0;
				qu.push(special);
			} else {
				currWidth++;
			}
		}
		return maxWidth;
	}

	// 判断是否为二叉搜索树
	static bool isBinarySearchTree(TreeNode *root) {
		if (root == nullptr)
			return true;
		bool currentIs = true;
		if (root->left != nullptr)
			currentIs = root->left->value <= root->value;
		if (root->right != nullptr)
			currentIs = currentIs && (root->value <= root->right->value);
		bool leftIs = isBinarySearchTree(root->left);
		bool rightIs = isBinarySearchTree(root->right);
		return currentIs && leftIs && rightIs;
	}

	// 判断是否为完全二叉树
	static bool isCompleteBinaryTree(TreeNode *root) {
		queue<TreeNode *> que;
		que.push(root);

		TreeNode *curr;
		bool mustBeLeft = false;
		while (!que.empty()) {
			curr = que.front(), que.pop();

			if (curr == nullptr)
				continue;

			if (curr->left == nullptr && curr->right != nullptr)
				return false;

			if (curr->left == nullptr || curr->right == nullptr)
				mustBeLeft = true;

			if (mustBeLeft && curr != nullptr && curr->left != nullptr && curr->right != nullptr)
				return false;

			que.push(curr->left);
			que.push(curr->right);
		}
		return true;
	}

	// = 二叉树套路就是问左右子树去拿信息, 所以左右子树除了返回一个bool标记装填
	// = 还需要返回一个额外的信息, 用于给当前的子树加工从而给上级递归返回当前子树额外的信息

	// 是否为满二叉树
	static pair<bool, int> isFullTree(TreeNode *root) {
		if (root == nullptr)
			return {true, 0};

		pair<bool, int> left = isFullTree(root->left);
		pair<bool, int> right = isFullTree(root->right);

		return {left.first && right.first && left.second == right.second, left.second + right.second + 1};
	}

	// 是否为平衡二叉树, 额外信息就是左右子树的高度
	static pair<bool, int> isBalancedTree(TreeNode *root) {
		if (root == nullptr)
			return {true, 0};

		pair<bool, int> leftResult = isBalancedTree(root->left);
		pair<bool, int> rightResult = isBalancedTree(root->right);

		return {leftResult.first && rightResult.first && std::abs(leftResult.second - rightResult.second) <= 1, std::max(leftResult.second, rightResult.second) + 1};
	}

	// 找到node1和node2最近的公共祖先
	// 首先是最普通的思路, 获得一个子节点到父节点的hashMap, 然后获得node1的父节点路径
	// node2再向上去回溯, 一旦有一个节点在node1的父节点路径中, 那么就直接返回这个节点
	static TreeNode *lowestCommonAncestorHashMap(TreeNode *root, TreeNode *node1, TreeNode *node2) {
		map<TreeNode *, TreeNode *> parentLookup;
		set<TreeNode *> parents{node1};

		// 需要把头节点提前压入
		getLookupMap(root, parentLookup);

		while (node1 != nullptr) {
			node1 = parentLookup[node1];
			parents.insert(node1);
		}

		set<TreeNode *>::iterator parentIter;
		while ((parentIter = parents.find(node2)) == parents.end())
			node2 = parentLookup[node2];

		return *parentIter;
	}

	// 先序遍历构建表
	static void getLookupMap(TreeNode *root, map<TreeNode *, TreeNode *> &parentLookup) {
		if (root == nullptr)
			return;
		if (root->left != nullptr)
			parentLookup[root->left] = root;
		if (root->right != nullptr)
			parentLookup[root->right] = root;
		getLookupMap(root->left, parentLookup);
		getLookupMap(root->right, parentLookup);
	}

	// 上面的思路虽然简单, 但是空间复杂度却是O(N)的, 有没有空间复杂度更好的算法?
	// 运用上面的思路, 想想当前节点需要向左子树和右子树去要什么信息?
	// 答案是直接去要公共祖先.
	// 如果node1和node2都在左子树或者右子树的话, 那么就问左边或者右边去要公共祖先
	// 如果node1在左子树, node2在右子树的话, 那么就当前节点就是公共祖先, 返回当前节点即可
	// 此外还要加上终止条件, 就是当前节点如果就是node1或者node2, 那么公共祖先就是自己, 所以返回自己
	static TreeNode *lowestCommonAncestor(TreeNode *root, TreeNode *node1, TreeNode *node2) {
		// 自己就是node1或者node2, 那么公共祖先就是自己
		if (root == nullptr || root == node1 || root == node2)
			return root;

		TreeNode *inLeft = lowestCommonAncestor(root->left, node1, node2);
		TreeNode *inRight = lowestCommonAncestor(root->right, node1, node2);

		// 如果一个在左一个在右, 返回当前节点即可
		if (inLeft != nullptr && inRight != nullptr)
			return root;

		// 向上返回公共祖先
		return inLeft != nullptr ? inLeft : inRight;
	}

	// 树型DP问题都可以用递归求解, 其余的就是分情况了
	static ParentTreeNode *getSuccessorNode(ParentTreeNode *root, ParentTreeNode *node) {
		if (node == nullptr)
			return nullptr;
		// 有右子树, 那么后继节点就是右子树的最左节点
		if (node->right != nullptr)
			return getLeftMost(node->right);
		// 没有右子树, 那么后继节点就是向上找
		else {
			ParentTreeNode *parent = node->parent;
			while (parent != nullptr && parent->left != node)
				node = parent, parent = node->parent;
			return parent;
		}
	}

	static ParentTreeNode *getLeftMost(ParentTreeNode *root) {
		if (root == nullptr)
			return nullptr;
		while (root->left != nullptr)
			root = root->left;
		return root;
	}

	// 折纸问题
	// 请把一段纸条竖着放在桌子上，
	// 然后从纸条的下边向上方对折1次，压出折痕后展开。此时折痕是凹下去的，即折痕突起的方向指向纸条的背面。
	// 如果从纸条的下边向上方连续对折2次，压出折痕后展开，此时有三条折痕，从上到下依次是下折痕、下折痕和上折痕。
	// 给定一个输入参数N，代表纸条都从下边向上方连续对折N次。请从上到下打印所有折痕的方向。
	// 例如:N=1时，打印:down, N=2时，打印:down down up
	// 折痕就是一个二叉树, 每个节点就是一个折痕, 左侧就是凹折痕, 右侧就是凸折痕, 总体顺序就是中序遍历

	// 这里使用i和N以及true打印出在我们脑海中模拟的树, 从而避免了构建真实的树所产生的额外的空间复杂度
	static void printAllFolds(int n) {
		printAllFoldsProcess(1, n, true);
	}

	// i 是节点的层数, N 是总层数, down表示是凹折痕还是凸折痕
	static void printAllFoldsProcess(int i, int N, bool down) {
		if (i > N)
			return;
		// 左节点
		printAllFoldsProcess(i + 1, N, true);
		cout << (down ? "凹" : "凸") << " ";
		// 右节点
		printAllFoldsProcess(i + 1, N, false);
	}
};

int main(int argc, char *argv[]) {
	TreeNode treeTraversal(vector<int>{1, 2, 3, 4, 5, 6, 7});

	// 深度优先的遍历
	TreeDepthFirstTraversal::preorderRecurr(&treeTraversal);
	cout << "\n";
	TreeDepthFirstTraversal::preorderStack(&treeTraversal);
	cout << "\n";
	TreeDepthFirstTraversal::inorderRecurr(&treeTraversal);
	cout << "\n";
	TreeDepthFirstTraversal::inorderStack(&treeTraversal);
	cout << "\n";
	TreeDepthFirstTraversal::postorderRecurr(&treeTraversal);
	cout << "\n";
	TreeDepthFirstTraversal::postorderStack(&treeTraversal);
	cout << "\n";

	// 广度优先的遍历
	TreeWidthFirstTraversal::widthFirstTraversal(&treeTraversal);
	cout << "\n";

	// 最大宽度
	TreeNode treeMaxWidth(vector<int>{1, 2, 3, 4, 5, 6, -1, -1, -1, 7, -1, -1, 8});
	cout << "Max Width: " << TreeProblems::maxWidth1(&treeMaxWidth) << "\n";
	cout << "Max Width: " << TreeProblems::maxWidth2(&treeMaxWidth) << "\n";

	// 是否为二叉搜索树
	TreeNode binarySearchTree(vector<int>{5, 3, 7, 2, 4, 6, 8, 1});
	cout << "Is binary search tree? " << std::boolalpha << TreeProblems::isBinarySearchTree(&binarySearchTree) << "\n";
	cout << "Is binary search tree? " << std::boolalpha << TreeProblems::isBinarySearchTree(&treeTraversal) << "\n";

	// 是否为完全二叉树
	TreeNode completeTree(vector<int>{1, 2, 3, 4, 5, 6});
	TreeNode notCompleteTree(vector<int>{1, 2, 3, -1, 5, 6});
	cout << "Is complete tree? " << std::boolalpha << TreeProblems::isCompleteBinaryTree(&completeTree) << "\n";
	cout << "Is complete tree? " << std::boolalpha << TreeProblems::isCompleteBinaryTree(&notCompleteTree) << "\n";

	// 是否为满二叉树
	TreeNode fullTree(vector<int>{1, 2, 3, 4, 5, 6, 7});
	TreeNode notFullTree(vector<int>{1, 2, 3, 4, 5, 6});
	cout << "is full tree? " << TreeProblems::isFullTree(&fullTree).first << "\n";
	cout << "is full tree? " << TreeProblems::isFullTree(&notFullTree).first << "\n";

	// 是否为平衡二叉树
	TreeNode balancedTree(vector<int>{1, 2, 3, 4, 5, 6, 7, 8});
	TreeNode notBalancedTree(vector<int>{1, 2, -1, 3, 4, -1, -1, 5});
	cout << "Is balanced tree? " << std::boolalpha << TreeProblems::isBalancedTree(&balancedTree).first << "\n";
	cout << "Is balanced tree? " << std::boolalpha << TreeProblems::isBalancedTree(&notBalancedTree).first << "\n";

	// 最近的公共祖先
	TreeNode ancestorTree(vector{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15});
	TreeNode *node1 = ancestorTree.left->right->left;
	TreeNode *node2 = ancestorTree.left->left->right;
	cout << "lowest common ancestor: " << TreeProblems::lowestCommonAncestorHashMap(&ancestorTree, node1, node2)->value << "\n";
	cout << "lowest common ancestor: " << TreeProblems::lowestCommonAncestor(&ancestorTree, node1, node2)->value << "\n";

	// 返回二叉树的后继节点. 在二叉树的中序遍历的序列中，node的下一个节点叫作node的后继节点
	// 因为中序遍历是按照左头右的顺序输出的, 因此后继节点就是下一个输出的节点
	ParentTreeNode parentTree(vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15});
	cout << "Successor Node: " << TreeProblems::getSuccessorNode(&parentTree, parentTree.left->left)->value << "\n";

	// 树的序列化, 如果标记了空三种任意的遍历顺序都可以, 如果没有的话就需要两个遍历结合
	TreeNode serialTree1(vector<int>{1, -1, 2, -1, -1, 3, -1});
	TreeNode serialTree2(vector<int>{1, 2, -1, -1, 3, -1, -1});
	string s1 = serialTree1.serial(), s2 = serialTree2.serial();
	cout << "Serialed Tree1: " << s1 << "\n";
	cout << "Serialed Tree2: " << s2 << "\n";
	cout << "Unserialed Tree1 Preorder: ";
	TreeDepthFirstTraversal::preorderRecurr(TreeNode::unserial(s1));
	cout << "\n";
	cout << "Unserialed Tree2 Preorder: ";
	TreeDepthFirstTraversal::preorderRecurr(TreeNode::unserial(s2));
	cout << "\n";

	TreeProblems::printAllFolds(2);

	return 0;
}
