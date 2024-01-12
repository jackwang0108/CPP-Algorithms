/*
 * @lc app=leetcode.cn id=136 lang=cpp
 *
 * [136] 只出现一次的数字
 */

#include <vector>

using std::vector;

// @lc code=start
class Solution
{
public:
    int singleNumber(vector<int> &nums)
    {
        int num = 0;
        for (int c : nums)
            num ^= c;
        return num;
    }
};
// @lc code=end
