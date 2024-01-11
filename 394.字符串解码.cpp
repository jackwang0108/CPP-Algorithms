/*
 * @lc app=leetcode.cn id=394 lang=cpp
 *
 * [394] 字符串解码
 */

#include <stack>
#include <cctype>
#include <string>
#include <iostream>
#include <algorithm>

using std::stack;
using std::string;

// @lc code=start
class Solution
{
public:
    string decodeString(string s)
    {
        stack<char> decodeStack;
        for (auto currChar : s)
        {
            if (currChar != ']')
            {
                decodeStack.push(currChar);
            }
            else
            {
                // 4[abc]: unitNum[unitStr]
                string unitStr = "";
                while (decodeStack.top() != '[')
                {
                    unitStr = decodeStack.top() + unitStr;
                    decodeStack.pop();
                }

                decodeStack.pop();

                string unitNum = "";
                while (!decodeStack.empty() && isdigit(decodeStack.top()))
                {
                    unitNum = decodeStack.top() + unitNum;
                    decodeStack.pop();
                }

                int num = std::stoi(unitNum);
                while (num-- > 0)
                    for (int c : unitStr)
                        decodeStack.push(c);
            }
        }

        s = "";
        while (!decodeStack.empty())
        {
            s = decodeStack.top() + s;
            decodeStack.pop();
        }
        return s;
    }
};
// @lc code=end
