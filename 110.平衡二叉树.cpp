/*
 * @lc app=leetcode.cn id=110 lang=cpp
 *
 * [110] 平衡二叉树
 */

#include <algorithm>

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
    int helper(TreeNode *root)
    {
        if (root == nullptr)
            return 0;
        int leftDepth = helper(root->left);
        int rightDepth = helper(root->right);
        if (leftDepth == -1 || rightDepth == -1 || std::abs(leftDepth - rightDepth) > 1)
            return -1;
        return std::max(leftDepth, rightDepth) + 1;
    }

    bool isBalanced(TreeNode *root)
    {
        if (root == nullptr)
            return true;
        return helper(root) != -1;
    }
};
// @lc code=end
