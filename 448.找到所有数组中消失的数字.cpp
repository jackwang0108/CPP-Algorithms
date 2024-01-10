/*
 * @lc app=leetcode.cn id=448 lang=cpp
 *
 * [448] 找到所有数组中消失的数字
 */

#include <vector>

using std::vector;

// @lc code=start
class Solution
{
public:
    // 负数防止溢出
    vector<int> findDisappearedNumbers(vector<int> &nums)
    {
        int value = 0;
        for (int i = 0; i < nums.size(); i++)
        {
            value = nums[i] < 0 ? -1 * nums[i] : nums[i];
            nums[value - 1] = nums[value - 1] < 0 ? nums[value - 1] : -1 * nums[value - 1];
        }
        vector<int> result;
        for (int i = 0; i < nums.size(); i++)
            if (nums[i] > 0)
                result.push_back(i + 1);
        return result;
    }
};
// @lc code=end

class Solution
{
    // 事实上因为范围限制溢出不了
    vector<int> findDisappearedNumbers(vector<int> &nums)
    {
        int n = nums.size();
        for (int i = 0; i < n; i++)
            nums[(nums[i] - 1) % n] += n;
        vector<int> result;
        for (int i = 0; i < n; i++)
            if (nums[i] <= n)
                result.push_back(i + 1);
        return result;
    }
};