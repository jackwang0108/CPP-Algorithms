/*
 * @lc app=leetcode.cn id=145 lang=cpp
 *
 * [145] 二叉树的后序遍历
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
    vector<int> postorderTraversal(TreeNode *root)
    {
        if (root == nullptr)
            return {};

        // speed up, don't check the children of leaf node
        if (root->left == nullptr && root->right == nullptr)
            return {root->val};

        vector<int> left = postorderTraversal(root->left);
        vector<int> right = postorderTraversal(root->right);
        left.insert(left.end(), right.cbegin(), right.cend());
        left.push_back(root->val);
        return left;
    }
};
// @lc code=end

class Solution
{
public:
    vector<int> postorderTraversal(TreeNode *root)
    {
        if (root == nullptr)
            return {};
        vector<int> ans;
        stack<TreeNode *> st;

        TreeNode *prevAccess = nullptr;
        // 后续遍历是在第三次访问到节点的时候再输出
        // 和前两个相比有些麻烦, 需要额外使用一个变量保存上一次访问的节点, 从而判断是否已经访问了右子树了
        while (root != nullptr || !st.empty())
        {
            // 直接降到底部, 同时是第一次访问
            while (root != nullptr)
            {
                st.push(root);
                root = root->left;
            }

            // 第二次访问
            root = st.top();
            st.pop();
            // 如果没有右子树则直接输出, 或者已经是第三次访问了
            if (root->right == nullptr || root->right == prevAccess)
            {
                ans.push_back(root->val);
                prevAccess = root;
                // 避免再次降到底部
                root = nullptr;
            }
            else
            {
                st.push(root);
                root = root->right;
            }
        }
        return ans;
    }
};