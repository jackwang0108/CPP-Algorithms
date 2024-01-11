/*
 * @lc app=leetcode.cn id=144 lang=cpp
 *
 * [144] 二叉树的前序遍历
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
    vector<int> preorderTraversal(TreeNode *root)
    {
        if (root == nullptr)
            return {};

        // speed up, don't check the children of leaf node
        if (root->left == nullptr && root->right == nullptr)
            return {root->val};

        vector<int> ans{root->val};
        vector<int> left = preorderTraversal(root->left);
        ans.insert(ans.end(), left.cbegin(), left.cend());
        vector<int> right = preorderTraversal(root->right);
        ans.insert(ans.end(), right.cbegin(), right.cend());
        return ans;
    }
};
// @lc code=end

class Solution
{
    vector<int> preorderTraversal(TreeNode *root)
    {
        if (root == nullptr)
            return {};
        vector<int> ans;
        stack<TreeNode *> st;
        // 先序遍历的关键在第一次访问的时候就直接输出
        while (root != nullptr || !st.empty())
        {
            // 直接降到底部
            while (root != nullptr)
            {
                ans.push_back(root->val);
                st.push(root);
                root = root->left;
            }
            root = st.top();
            st.pop();
            root = root->right;
        }
        return ans;
    }
};