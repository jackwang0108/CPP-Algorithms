/*
 * @lc app=leetcode.cn id=461 lang=cpp
 *
 * [461] 汉明距离
 */

// @lc code=start
class Solution
{
public:
    int hammingDistance(int x, int y)
    {
        int distance = 0;
        for (int xorans = x ^ y; xorans != 0; xorans &= (xorans - 1))
            distance++;
        return distance;
    }
};
// @lc code=end
