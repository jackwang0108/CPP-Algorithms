/*
 * @lc app=leetcode.cn id=3 lang=cpp
 *
 * [3] 无重复字符的最长子串
 */

#include <string>
#include <vector>
#include <algorithm>

using std::string;
using std::vector;

// @lc code=start
class Solution
{
public:
    int lengthOfLongestSubstring(string s)
    {
        int maxLen = 0, subStrPos = 0;
        vector<char> subStr;
        for (int i = 0; i < s.size(); i++)
        {
            while (std::find(subStr.begin() + subStrPos, subStr.end(), s[i]) != subStr.end())
                subStrPos++;
            subStr.push_back(s[i]);
            maxLen = std::max(maxLen, i - subStrPos + 1);
        }
        return maxLen;
    }
};
// @lc code=end
