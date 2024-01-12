/*
 * @lc app=leetcode.cn id=20 lang=cpp
 *
 * [20] 有效的括号
 */
#include <stack>
#include <string>

using std::stack;
using std::string;

// @lc code=start
class Solution
{
public:
    bool isValid(string s)
    {
        stack<char> st;
        for (char c : s)
        {
            if (!st.empty() && c == ']')
                if (st.top() != '[')
                    return false;
                else
                    st.pop();
            else if (!st.empty() && c == ')')
                if (st.top() != '(')
                    return false;
                else
                    st.pop();
            else if (!st.empty() && c == '}')
                if (st.top() != '{')
                    return false;
                else
                    st.pop();
            else
                st.push(c);
        }
        return st.empty();
    }

    // 更高效的写法
    bool isValid(string s)
    {
        stack<char> st;
        for (char c : s)
        {
            if (c == '[')
                st.push(']');
            else if (c == '{')
                st.push('}');
            else if (c == '[')
                st.push(']');
            else if (st.empty() || st.top() != c)
                return false;
            else
                st.pop();
        }
        return st.empty();
    }
};
// @lc code=end
