/*
 * @lc app=leetcode.cn id=415 lang=cpp
 *
 * [415] 字符串相加
 */
#include <stack>
#include <string>

using std::stack;
using std::string;

// @lc code=start
class Solution
{
public:
    string addStrings(string num1, string num2)
    {
        int carry = 0;
        string ans;
        string &shorter = num1.size() < num2.size() ? num1 : num2;
        string &longer = num1.size() >= num2.size() ? num1 : num2;
        int diff = longer.size() - shorter.size();
        int i, sum;
        for (i = longer.size() - 1; i >= 0; i--)
        {
            if (i - diff >= 0)
                sum = longer[i] + shorter[i - diff] - '0' - '0' + carry;
            else
                sum = longer[i] - '0' + carry;
            char remain = (sum % 10 + '0');
            carry = sum / 10;
            ans = remain + ans;
        }
        return carry == 1 ? '1' + ans : ans;
    }

    // 更优雅的解法
    string addStrings(string num1, string num2)
    {
        string ans;
        int carry = 0;
        for (int i = num1.size() - 1, j = num2.size() - 1; i >= 0 || j >= 0 || carry == 1; i--, j--)
        {
            int x = i < 0 ? 0 : num1[i] - '0';
            int y = j < 0 ? 0 : num2[j] - '0';
            ans.insert(ans.begin(), (x + y + carry) % 10 + '0');
            carry = (x + y + carry) / 10;
        }
        return ans;
    }
};
// @lc code=end
