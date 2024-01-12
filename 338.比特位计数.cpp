/*
 * @lc app=leetcode.cn id=338 lang=cpp
 *
 * [338] 比特位计数
 */
#include <vector>

using std::vector;

// @lc code=start
class Solution
{
public:
    vector<int> countBits(int n)
    {
        vector<int> ans(n + 1, 0);
        for (int i = 1; i <= n; i++)
            ans[i] = ans[i & (i - 1)] + 1;
        return ans;
    }
};
// @lc code=end
