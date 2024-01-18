#include <map>
#include <stack>
#include <queue>
#include <vector>
#include <utility>
#include <iostream>

using std::cout;
using std::endl;
using std::map;
using std::pair;
using std::queue;
using std::stack;
using std::vector;

struct TreeNode
{
    int value;
    TreeNode *left;
    TreeNode *right;

    TreeNode() : value(0), left(nullptr), right(nullptr) {}

    explicit TreeNode(int data) : value(data), left(nullptr), right(nullptr) {}

    explicit TreeNode(vector<int> &&vec) : value(0), left(nullptr), right(nullptr)
    {
        if (vec.empty())
        {
            return;
        }
        value = vec[0];
        left = buildTree(vec, 1);
        right = buildTree(vec, 2);
    }

    TreeNode *buildTree(vector<int> &vec, int rootIdx)
    {
        if (rootIdx >= vec.size())
            return nullptr;
        int leftChildIdx = rootIdx * 2 + 1, rightChildIdx = rootIdx * 2 + 2;
        TreeNode *root = new TreeNode;

        if (vec[rootIdx] == -1)
            return nullptr;

        root->value = vec[rootIdx];
        root->left = buildTree(vec, leftChildIdx);
        root->right = buildTree(vec, rightChildIdx);
        return root;
    }
};

class TreeDepthFirstTraversal
{
public:
    // 先序遍历递归版
    static void preorderRecurr(TreeNode *root)
    {
        if (root == nullptr)
            return;
        cout << root->value << " ";
        preorderRecurr(root->left);
        preorderRecurr(root->right);
    }

    // 中序遍历递归版
    static void inorderRecurr(TreeNode *root)
    {
        if (root == nullptr)
            return;
        inorderRecurr(root->left);
        cout << root->value << " ";
        inorderRecurr(root->right);
    }

    // 后序遍历递归版
    static void postorderRecurr(TreeNode *root)
    {
        if (root == nullptr)
            return;
        postorderRecurr(root->left);
        postorderRecurr(root->right);
        cout << root->value << " ";
    }

    // 先序遍历非递归版
    static void preorderStack(TreeNode *root)
    {
        stack<TreeNode *> st;
        st.push(root);

        TreeNode *curr;
        while (!st.empty())
        {
            curr = st.top(), st.pop();
            if (curr == nullptr)
                continue;
            cout << curr->value << " ";
            st.push(curr->right);
            st.push(curr->left);
        }
    }

    // 中序遍历非递归版
    static void inorderStack(TreeNode *root)
    {
        // 中序遍历, 先把左孩子进栈, 直到左孩子为空, 然后右孩子进栈, 然后对右孩子重复这个处理
        stack<TreeNode *> st;
        while (!st.empty() || root != nullptr)
        {
            if (root != nullptr)
            {
                st.push(root);
                root = root->left;
            }
            else
            {
                root = st.top(), st.pop();
                cout << root->value << " ";
                root = root->right;
            }
        }
    }

    // 后序遍历非递归版
    static void postorderStack(TreeNode *root)
    {
        // 后序遍历输出顺序是左右头, 再加一个收集栈反向之后, 只要打印顺序是头右左即可
        stack<TreeNode *> st, print;
        st.push(root);

        TreeNode *curr;
        while (!st.empty())
        {
            curr = st.top(), st.pop();
            if (curr == nullptr)
                continue;
            print.push(curr);
            st.push(curr->left);
            st.push(curr->right);
        }

        while (!print.empty())
        {
            curr = print.top(), print.pop();
            cout << curr->value << " ";
        }
    }
};

// 广度优先遍历
class TreeWidthFirstTraversal
{
public:
    static void widthFirstTraversal(TreeNode *root)
    {
        queue<TreeNode *> st;
        st.push(root);

        TreeNode *curr;
        while (!st.empty())
        {
            curr = st.front(), st.pop();
            if (curr == nullptr)
                continue;
            cout << curr->value << " ";
            st.push(curr->left);
            st.push(curr->right);
        }
    }
};

class TreeProblems
{
public:
    // 求一个树的最大宽度
    static int maxWidth1(TreeNode *root)
    {
        queue<TreeNode *> q;
        map<TreeNode *, int> levelMap;

        q.push(root);
        levelMap.insert(std::make_pair(root, 1));
        TreeNode *curr;
        int currDepth = 1, currWidth = 0, maxWidth = 0;
        while (!q.empty())
        {
            curr = q.front(), q.pop();
            if (curr == nullptr)
                continue;

            int currNodeDepth = levelMap[curr];
            if (currNodeDepth == currDepth)
                currWidth++;
            else
            {
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
    static int maxWidth2(TreeNode *root)
    {
        queue<TreeNode *> qu;

        qu.push(root);
        TreeNode *curr, *currEnd = root, *special = new TreeNode;
        qu.push(special);
        int currDepth = 1, currWidth = 0, maxWidth = 0;
        while (qu.size() > 1)
        {
            curr = qu.front(), qu.pop();

            if (curr->left != nullptr)
                qu.push(curr->left);
            if (curr->right != nullptr)
                qu.push(curr->right);

            // 判断是否为特殊标记的尾部
            if (curr == special)
            {
                currDepth++;
                maxWidth = std::max(maxWidth, currWidth);
                currWidth = 0;
                qu.push(special);
            }
            else
            {
                currWidth++;
            }
        }
        return maxWidth;
    }

    // 判断是否为二叉搜索树
    static bool isBinarySearchTree(TreeNode *root)
    {
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
    static bool isCompleteBinaryTree(TreeNode *root)
    {
        queue<TreeNode *> que;
        que.push(root);

        TreeNode *curr;
        bool mustBeLeft = false;
        while (!que.empty())
        {
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
    static pair<bool, int> isFullTree(TreeNode *root)
    {
        if (root == nullptr)
            return {true, 0};

        pair<bool, int> left = isFullTree(root->left);
        pair<bool, int> right = isFullTree(root->right);

        return {left.first && right.first && left.second == right.second, left.second + right.second + 1};
    }

    // 是否为平衡二叉树, 额外信息就是左右子树的高度
    static pair<bool, int> isBalancedTree(TreeNode *root)
    {
        if (root == nullptr)
            return {true, 0};

        pair<bool, int> leftResult = isBalancedTree(root->left);
        pair<bool, int> rightResult = isBalancedTree(root->right);

        return {leftResult.first && rightResult.first && std::abs(leftResult.second - rightResult.second) <= 1, std::max(leftResult.second, rightResult.second) + 1};
    }
};

int main(int argc, char *argv[])
{
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
    return 0;
}
