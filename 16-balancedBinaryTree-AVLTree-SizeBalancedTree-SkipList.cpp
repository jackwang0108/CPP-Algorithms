#include <iostream>
#include <memory>
#include <vector>

using std::cout;
using std::shared_ptr;
using std::unique_ptr;
using std::vector;
using std::weak_ptr;

// #define DEBUG

struct TreeNode {
	int value;
	weak_ptr<TreeNode> parent;
	shared_ptr<TreeNode> left;
	shared_ptr<TreeNode> right;

	TreeNode() {}
	explicit TreeNode(int value) : value(value) {}

#ifdef DEBUG
	~TreeNode() { cout << "delete: " << value << "\n"; }
#endif
};

// 我们在前面介绍了哈希表的实现, 即unordered_map, 其内部是使用哈希函数+取模实现的均匀映射和范围限制, 再加上限制链表长度, 从而做到了O(1)时间复杂度的访问
// 我们这节课介绍一下有序表, 即map的实现

// 有序表的实现可以采用: 红黑树, AVL树, SB树, 跳表
// 这几个数据结构的时间复杂度都是一样的, 区别非常小, 只是在常数时间上存在差别
// 打竞赛的人一般喜欢用SB树, 因为这个好改, 并不是说SB树就是最好的

// 红黑树, AVL树, SB树都是平衡搜索二叉树的分支, Balanced Search Tree
// 跳表则是另外一种实现方式

// 本节课讲授的内容如下:
//  ,--------------------------------------------------------------,
//  |                    ,------------------------,                |
//  |                    | ,--------------------, |                |
//  |                    | | 二叉搜索树 (增删改查)  | |                |
//  |                    | '--------------------' |                |
//  |                    | 自平衡操作: 左旋 + 右旋    |                |
//  |                    '------------------------'                |
//  | AVL树: 在左旋右旋基础上定义如何使用左旋右旋操作                      |
//  | 红黑树: 有自己对平衡性的定义, 并且在自己的平衡性的定义下如何维持平衡性    |
//  | SB树: 有自己对平衡性的定义, 并且在自己的平衡性的定义下如何维持平衡性     |
//  '--------------------------------------------------------------'

// 首先来看一下二叉搜索树的增删改查操作, 前面只讲了树的遍历操作, 并没有讲树的增删改查操作
class BinarySearchTree {
public:
	int size = 0;
	shared_ptr<TreeNode> root = nullptr;

	BinarySearchTree() {}
	explicit BinarySearchTree(const vector<int> &vec) {
		for (auto i: vec)
			insert(i);
	}

	// 增加操作
	shared_ptr<TreeNode> insertNode(TreeNode &node) { return insert(node.value); }
	shared_ptr<TreeNode> insert(int value) {
		if (root == nullptr) {
			root = std::make_shared<TreeNode>(value);
			size++;
			return root;
		}

		// 搜索到curr为空, 每次走的时候记录一下父节点
		shared_ptr<TreeNode> parent;
		shared_ptr<TreeNode> curr = root;
		while (curr != nullptr) {
			parent = curr;
			if (value < curr->value)
				curr = curr->left;
			else
				curr = curr->right;
		}

		// 插入节点
		size++;
		if (parent->value > value) {
			parent->left = std::make_shared<TreeNode>(value);
			parent->left->parent = parent;
			return parent->left;
		} else {
			parent->right = std::make_shared<TreeNode>(value);
			parent->right->parent = parent;
			return parent->right;
		}
	}

	// 查询操作
	shared_ptr<TreeNode> findNode(int value) {
		shared_ptr<TreeNode> curr = root;
		while (curr != nullptr && curr->value != value) {
			if (value < curr->value)
				curr = curr->left;
			else
				curr = curr->right;
		}
		return curr;
	}

	// 删除操作
	shared_ptr<TreeNode> deleteNode(TreeNode &node) { return deleteNode(node.value); }
	shared_ptr<TreeNode> deleteNode(int value) {
		// 首先要找到当前节点
		shared_ptr<TreeNode> nodeToDelete = findNode(value);
		if (nodeToDelete == nullptr)
			return nullptr;

		// 然后再删除节点
		// 情况一: 只有一个孩子, 这里是左孩子
		if (nodeToDelete->left == nullptr)
			;
		return root;
	}

	// 移植以newNode为根节点的子树到nodeToReplace所指向的节点上
	shared_ptr<TreeNode> transplant(shared_ptr<TreeNode> nodeToReplace, shared_ptr<TreeNode> newNode) {
		// if (nodeToReplace->parent.lock() == nullptr)
		// 	this->root = newNode;
		// else if (nodeToReplace == nodeToReplace->parent.lock()->left)
		// 	nodeToReplace;
		return newNode;
	}
};


int main(int argc, char *argv[]) {
	BinarySearchTree binarySearchTree(vector<int>{4, 1, 2, 3, 5, 6, 7});
	return 0;
}