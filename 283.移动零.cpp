/*
 * @lc app=leetcode.cn id=283 lang=cpp
 *
 * [283] 移动零
 */

#include <vector>

using std::vector;

// @lc code=start
class Solution
{
public:
    // 时间复杂度: O(N), 空间复杂度: O(1)
    void moveZeroes(vector<int> &nums)
    {
        int nonZeroNums = 0, movingPtr = 0;
        while (movingPtr < nums.size())
        {
            if (nums[movingPtr] == 0)
                movingPtr++;
            else
                nums[nonZeroNums++] = nums[movingPtr++];
        }
        while (nonZeroNums < nums.size())
            nums[nonZeroNums++] = 0;
    }
};
// @lc code=end
