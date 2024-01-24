#include <iostream>
#include <memory>
#include <vector>

using std::cout;
using std::shared_ptr;
using std::vector;
using std::weak_ptr;

struct TreeNode {
	int value;
	TreeNode *parent;
	TreeNode *left;
	TreeNode *right;

	TreeNode() : value(0), parent(nullptr), left(nullptr), right(nullptr) {}
	explicit TreeNode(int value) : value(value), parent(nullptr), left(nullptr), right(nullptr) {}

	explicit TreeNode(vector<int> &&vec) : value(0), parent(nullptr), left(nullptr), right(nullptr) {
		if (vec.empty())
			return;

		value = vec[0];
		left = buildTree(vec, 1, this);
		right = buildTree(vec, 2, this);
	}

	TreeNode *buildTree(vector<int> &vec, int rootIdx, TreeNode *parent) {
		if (rootIdx >= vec.size() || vec[rootIdx] == -1)
			return nullptr;
		int leftChildIdx = 2 * rootIdx + 1,
		    rightChildIdx = 2 * rootIdx + 2;
		TreeNode *root = new TreeNode;

		root->value = vec[rootIdx];
		root->parent = parent;
		root->left = buildTree(vec, leftChildIdx, root);
		root->right = buildTree(vec, rightChildIdx, root);
		return root;
	}
};


// = 树形dp套路
// 树形dp套路使用前提：如果题目求解目标是S规则，则求解流程可以定成以每一个节点为头节点的子树在S规则下的每一个答案，并且最终答案一定在其中
// = 树形dp套路第一步：
//      以某个节点X为头节点的子树中，分析答案有哪些可能性，并且这种分析是以X的左子树、X的右子树和X整棵树的角度来考虑可能性的
//   可能性的分类有很多种, 例如头结点是否参与
// = 树形dp套路第二步：
//      根据第一步的可能性分析，列出所有需要的信息
// = 树形dp套路第三步：
//      合并第二步的信息，对左树和右树提出同样的要求，并写出信息结构
// = 树形dp套路第四步：
//      设计递归函数，递归函数是处理以X为头节点的情况下的答案。包括设计递归的basecase，默认直接得到左树和右树的所有信息，以及把可能性做整合，并且要返回第三步的信息结构这四个小步骤

struct Employee {
	int happy;
	vector<Employee> subordinates;

	Employee() {}
	Employee(int happy, vector<Employee> subordinates) : happy(happy), subordinates(subordinates) {}
};


// 先以一个题引入树形DP问题
class TreeDPProblems {
public:
	struct MaxDepthInTreeInfo {
		int height = 0;
		int maxDepth = 0;
	};
	// 二叉树节点间的最大距离问题
	// 从二叉树的节点a出发，可以向上或者向下走，但沿途的节点只能经过一次，到达节点b时路径上的节点个数叫作a到b的距离，那么二叉树任何两个节点之间都有距离，求整棵树上的最大距离。
	// 第一步首先分析情况
	//      头结点不参与, 那么最大距离要么来自左树, 要么来自右树
	//      头结点参与, 那么最大距离就是左树高度+右树高度+1
	//      所以利用头节点是否参与, 我们就已经讨论了所有可能的情况
	// 递归策略, 所以递归过程中, 我们需要返回高度和最大距离两个信息
	static int maxDistanceInTree(TreeNode *root) {
		return processMaxDistanceInTree(root).maxDepth;
	}

	static MaxDepthInTreeInfo processMaxDistanceInTree(TreeNode *root) {
		if (root == nullptr)
			return {0, 0};
		MaxDepthInTreeInfo leftInfo = processMaxDistanceInTree(root->left);
		MaxDepthInTreeInfo rightInfo = processMaxDistanceInTree(root->right);
		MaxDepthInTreeInfo result{
		    .height = std::max(leftInfo.height, rightInfo.height) + 1,
		    .maxDepth = std::max(leftInfo.height + rightInfo.height + 1, std::max(leftInfo.maxDepth, rightInfo.maxDepth))};
		return result;
	}


	// = 派对的最大快乐值
	// 员工信息的定义如下:
	// class Employee{
	//      public int happy;  //这名员工可以带来的快乐值
	//      List<Employee> subordinates;  //这名员工有哪些直接下级
	// }
	// 公司的每个员工都符合Employee类的描述。
	// 整个公司的人员结构可以看作是一棵标准的、没有环的多叉树。 树的头节点是公司唯一的老板。 除老板之外的每个员工都有唯一的直接上级。 叶节点是没有任何下属的基层员工(subordinates列表为空)，除基层员工外，每个员工都有一个或多个直接下级。
	// 这个公司现在要办party，你可以决定哪些员工来，哪些员工不来。但是要遵循如下规则。
	//      1.如果某个员工来了，那么这个员工的所有直接下级都不能来, 但是下级的下级可以来
	//      2.派对的整体快乐值是所有到场员工快乐值的累加
	//      3.你的目标是让派对的整体快乐值尽量大
	// 给定一棵多叉树的头节点boss，请返回派对的最大快乐值。
	//
	// 分析:  x -> {a,b,c},
	//              a -> {1, 2}
	//              b -> {3, 4}
	//              c -> {5, 6}
	// 所有的情况:
	//      1. x来了, 那么欢乐值就是 自己的欢乐值 + a整棵树a不来的最大欢乐值 + b整棵树... + c整棵树...
	//      2. x不来, 那么欢乐值就是 max{a来,a不来}整棵树的最大换乐值 + max{b...}..., + max{c...}...
	// 所以想每棵子树要的信息就是子树的头来和不来的最大欢乐值
	struct MaxHappyInfo {
		int withLeaderHappy = 0;
		int withoutLeaderHappy = 0;
	};

	static int maxHappy(Employee *boss) {
		MaxHappyInfo bossInfo = processMaxHappy(boss);
		return std::max(bossInfo.withLeaderHappy, bossInfo.withoutLeaderHappy);
	}

	static MaxHappyInfo processMaxHappy(Employee *leader) {
		if (leader->subordinates.empty())
			return MaxHappyInfo{leader->happy, 0};

		MaxHappyInfo currLeaderInfo{
		    .withoutLeaderHappy = 0,
		    .withLeaderHappy = leader->happy};
		for (Employee next: leader->subordinates) {
			MaxHappyInfo nextInfo = processMaxHappy(&next);
			currLeaderInfo.withLeaderHappy += nextInfo.withoutLeaderHappy;
			currLeaderInfo.withoutLeaderHappy += std::max(nextInfo.withLeaderHappy, nextInfo.withoutLeaderHappy);
		}

		return currLeaderInfo;
	}
};


class MorrisTraversal {
	// =Morris遍历
	// 一种遍历二叉树的方式，并且时间复杂度O(N)，额外空间复杂度O(1)
	// 通过利用原树中大量空闲指针的方式，达到节省空间的目的
	// 很多二叉树的问题都是遍历一边二叉树就能解决了的, 而我们前面实现的二叉树的遍历, 无论是递归还是手动压栈
	// 空间复杂度都是O(N), 但是如果我们能够实现一种空间复杂度O(1)的二叉树遍历方式, 那就意味着我们的解法实际上就是最优秀的解法
	// Morris遍历不是用在笔试的, 因为代码比前面的递归方式麻烦, 主要用在面试阶段
	// = 如果一棵树遍历的时候禁止修改这棵树, 那么Morris遍历就用不了, 但是如果这棵树在遍历的过程中允许修改指针, 只要遍历结束再修改回去
	// = 那么就可以使用Morris遍历

	// = Morris遍历细节
	// 假设来到当前节点cur，开始时cur来到头节点位置
	//  1）如果cur没有左孩子，cur向右移动(cur=cur.right)
	//  2）如果cur有左孩子，找到左子树上最右的节点mostRight：
	//      a.如果mostRight的右指针指向空，让其指向cur，然后cur向左移动(cur=cur.left)
	//      b.如果mostRight的右指针指向cur，让其指向null，然后cur向右移动(cur=cur.right)
	//  3）cur为空时遍历停止
	// 如果一个节点有左子树, 那么Morris遍历会来到这个节点两次, 可以根据左子树左右节点的右指针来判断是第几次来到当前节点
	// 从而加工得到前序, 中序和后序遍历

	// = 时间复杂度估计, 最重要的是找到左子树的右节点, 这个就是用左边界划分整个树, 所以时间复杂度就是O(N)级别

	// = 什么情况下Morris遍历是最优解? 或者说什么时候必须用递归, 什么时候用Morris遍历?
	// = 答案是如果遍历过程中需要在第三次遍历结束后才能进行信息的强整合, 那么就必须用递归, 否则Morris遍历就是最优解
public:
	static void morris(TreeNode *tree) {
		if (tree == nullptr)
			return;
		TreeNode *curr = tree, *mostRight = nullptr;
		while (curr != nullptr) {
			mostRight = curr->left;
			if (mostRight != nullptr) {
				// 找到左子树最右节点
				while (mostRight->right != nullptr && mostRight->right != curr)
					mostRight = mostRight->right;
				// 左子树最右节点第一次访问到
				if (mostRight->right == nullptr) {
					// 情况2的a, 第一次访问当前节点
					cout << curr->value << " ";
					mostRight->right = curr;
					curr = curr->left;
					continue;
				} else {
					// 情况2的b
					mostRight->right = nullptr;
				}
			}
			// 访问当前节点
			cout << curr->value << " ";
			curr = curr->right;
		}
	}

	// Morris遍历的基础上改先序遍历, 就是
	// 若节点A会被经历两次, 那么只有第一次打印, 如果只经历一次, 那么直接打印
	static void morrisPreOrder(TreeNode *tree) {
		if (tree == nullptr)
			return;
		TreeNode *curr = tree, *mostRight = nullptr;
		while (curr != nullptr) {
			mostRight = curr->left;
			if (mostRight != nullptr) {
				// 找到左子树最右节点
				while (mostRight->right != nullptr && mostRight->right != curr)
					mostRight = mostRight->right;
				// 左子树最右节点第一次访问到
				if (mostRight->right == nullptr) {
					// 情况2的a, 第一次访问当前节点
					cout << curr->value << " ";
					mostRight->right = curr;
					curr = curr->left;
					continue;
				} else {
					// 情况2的b
					mostRight->right = nullptr;
				}
			} else
				// 没有左子树, 只会访问当前节点一次, 所以直接打印
				cout << curr->value << " ";
			curr = curr->right;
		}
	}

	// Morris遍历的基础上改中序遍历, 就是
	// 若节点A会被经历两次, 那么只有第二次打印, 如果只经历一次, 那么直接打印
	static void morrisInOrder(TreeNode *tree) {
		if (tree == nullptr)
			return;
		TreeNode *curr = tree, *mostRight = nullptr;
		while (curr != nullptr) {
			mostRight = curr->left;
			if (mostRight != nullptr) {
				// 找到左子树最右节点
				while (mostRight->right != nullptr && mostRight->right != curr)
					mostRight = mostRight->right;
				// 左子树最右节点第一次访问到
				if (mostRight->right == nullptr) {
					// 情况2的a, 第一次访问当前节点
					mostRight->right = curr;
					curr = curr->left;
					continue;
				} else {
					// 情况2的b
					mostRight->right = nullptr;
				}
			}
			// 没有左子树, 只会访问当前节点一次, 所以直接打印
			cout << curr->value << " ";
			curr = curr->right;
		}
	}

	static TreeNode *reverseRightEdge(TreeNode *tree) {
		TreeNode *prev = nullptr, *next = nullptr;
		while (tree != nullptr) {
			next = tree->right;
			tree->right = prev;
			prev = tree;
			tree = next;
		}
		return prev;
	}

	static void printRightEdgeReversely(TreeNode *root) {
		TreeNode *reversed = reverseRightEdge(root), *copy = reversed;
		while (copy != nullptr) {
			cout << copy->value << " ";
			copy = copy->right;
		}
		reverseRightEdge(reversed);
	}

	// Morris遍历的基础上改后序遍历, 就是
	// 后序遍历有些tricky, 因为Morris遍历一个节点只会访问到两次
	// 后序遍历是在能访问到的节点第二次访问的时候逆序打印左子树右边界
	// 最后打印整棵树的右边界
	static void morrisPostOrder(TreeNode *tree) {
		if (tree == nullptr)
			return;
		TreeNode *curr = tree, *mostRight = nullptr;
		while (curr != nullptr) {
			mostRight = curr->left;
			if (mostRight != nullptr) {
				// 找到左子树最右节点
				while (mostRight->right != nullptr && mostRight->right != curr)
					mostRight = mostRight->right;
				// 左子树最右节点第一次访问到
				if (mostRight->right == nullptr) {
					// 情况2的a, 第一次访问当前节点
					mostRight->right = curr;
					curr = curr->left;
					continue;
				} else {
					// 情况2的b
					mostRight->right = nullptr;
					printRightEdgeReversely(curr->left);
				}
			} else
				;
			curr = curr->right;
		}
		printRightEdgeReversely(tree);
	}

	// 检查是否是二叉搜索树就是中序遍历序列是升序的, 如果递归的中序遍历还需要一个数组存储中序遍历得到的序列
	// 如果是morris的话就不用了
	static bool isBST(TreeNode *tree) {
		if (tree == nullptr)
			return false;
		int prevValue = INT_MIN;
		TreeNode *curr = tree, *mostRight = nullptr;
		while (curr != nullptr) {
			mostRight = curr->left;
			if (mostRight != nullptr) {
				// 找到左子树最右节点
				while (mostRight->right != nullptr && mostRight->right != curr)
					mostRight = mostRight->right;
				// 左子树最右节点第一次访问到
				if (mostRight->right == nullptr) {
					// 情况2的a, 第一次访问当前节点
					mostRight->right = curr;
					curr = curr->left;
					continue;
				} else {
					// 情况2的b
					mostRight->right = nullptr;
				}
			}
			if (curr->value <= prevValue)
				return false;
			prevValue = curr->value;
			curr = curr->right;
		}
		return true;
	}
};


int main(int argc, char *argv[]) {
	// 树的最远距离
	TreeNode maxDistanceTree(vector<int>{1, 2, 3, 4, -1, -1, 5, -1, -1, -1, -1, -1, -1, 6, 7});
	cout << "MaxDistanceInTree: " << TreeDPProblems::maxDistanceInTree(&maxDistanceTree) << "\n";

	// 排队最大欢乐值
	Employee boss{10, vector<Employee>{}};
	boss.subordinates.emplace_back(3, vector<Employee>{});
	boss.subordinates.back().subordinates.emplace_back(60, vector<Employee>{});
	boss.subordinates.emplace_back(20, vector<Employee>{});
	boss.subordinates.back().subordinates.emplace_back(3, vector<Employee>{});
	boss.subordinates.emplace_back(40, vector<Employee>{});
	boss.subordinates.back().subordinates.emplace_back(5, vector<Employee>{});
	boss.subordinates.back().subordinates.emplace_back(6, vector<Employee>{});
	cout << "MaxHappy: " << TreeDPProblems::maxHappy(&boss) << "\n";

	// Morris遍历
	TreeNode root(vector<int>{1, 2, 3, 4, 5, 6, 7});
	cout << "Morris Traversal: ";
	MorrisTraversal::morris(&root);
	cout << "\n";

	// Morris遍历改先序遍历
	cout << "Morris PreOrder Traversal: ";
	MorrisTraversal::morrisPreOrder(&root);
	cout << "\n";
	// Morris遍历改中序遍历
	cout << "Morris InOrder Traversal: ";
	MorrisTraversal::morrisInOrder(&root);
	cout << "\n";
	// Morris遍历改后序遍历
	cout << "Morris PostOrder Traversal: ";
	MorrisTraversal::morrisPostOrder(&root);
	cout << "\n";


	// 是否为二叉搜索树
	TreeNode binarySearchTree(vector<int>{5, 3, 7, 2, 4, 6, 8, 1});
	TreeNode treeTraversal(vector<int>{1, 2, 3, 4, 5, 6, 7});
	cout << "Is binary search tree? " << std::boolalpha << MorrisTraversal::isBST(&binarySearchTree) << "\n";
	cout << "Is binary search tree? " << std::boolalpha << MorrisTraversal::isBST(&treeTraversal) << "\n";
	return 0;
}
