/*
 * @lc app=leetcode.cn id=104 lang=cpp
 *
 * [104] 二叉树的最大深度
 */

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
    int maxDepthCounter(TreeNode *root, int currDepth)
    {
        if (root == nullptr)
            return currDepth;
        int leftDepth = maxDepthCounter(root->left, currDepth + 1);
        int rightDepth = maxDepthCounter(root->right, currDepth + 1);
        return leftDepth > rightDepth ? leftDepth : rightDepth;
    }

    int maxDepth(TreeNode *root)
    {
        if (root == nullptr)
            return 0;
        return maxDepthCounter(root, 0);
    }
};
// @lc code=end
