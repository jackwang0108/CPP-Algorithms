/*
 * @lc app=leetcode.cn id=94 lang=cpp
 *
 * [94] 二叉树的中序遍历
 */

#include <stack>
#include <vector>

using std::stack;
using std::vector;

struct TreeNode
{
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// @lc code=start
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution
{
public:
    // 递归解法, 很简单
    vector<int> inorderTraversal1(TreeNode *root)
    {
        if (root == nullptr)
            return {};

        vector<int> left = inorderTraversal1(root->left);
        left.insert(left.end(), root->val);
        vector<int> right = inorderTraversal1(root->right);
        left.insert(left.end(), right.cbegin(), right.cend());
        return left;
    }

    // 高效版, 使用引用避免复制
    vector<int> inorderTraversal(TreeNode *root)
    {
        vector<int> ans;
        inorderTraverser(root, ans);
        return ans;
    }

    void inorderTraverser(TreeNode *root, vector<int> &ans)
    {
        if (root == nullptr)
            return;
        inorderTraverser(root->left, ans);
        ans.insert(ans.end(), root->val);
        inorderTraverser(root->right, ans);
    }
};
// @lc code=end

class Solution
{
    // 非递归解法, 需要使用栈
    vector<int> inorderTraversal(TreeNode *root)
    {
        vector<int> ans;
        stack<TreeNode *> st;
        // 中序遍历的关键在第一次访问的时候不输出, 第二次访问再输出
        while (root != nullptr || !st.empty())
        {
            // 直接降到底部
            while (root != nullptr)
            {
                // 第一次访问不输出
                st.push(root);
                root = root->left;
            }
            root = st.top();
            st.pop();
            // 第二次访问再输出
            ans.push_back(root->val);
            root = root->right;
        }
        return ans;
    }
};