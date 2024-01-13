/*
 * @lc app=leetcode.cn id=9 lang=cpp
 *
 * [9] 回文数
 */

// @lc code=start
class Solution
{
public:
    bool isPalindrome(int x)
    {
        if (x < 0)
            return false;
        if (x == 0)
            return true;
        int copy = x;
        long reverse = 0;
        while (x > 0)
        {
            reverse = (x % 10) + reverse * 10;
            x /= 10;
        }
        return copy == reverse;
    }
};
// @lc code=end
