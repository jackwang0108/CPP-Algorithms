/*
 * @lc app=leetcode.cn id=70 lang=cpp
 *
 * [70] 爬楼梯
 */

#include <vector>
#include <utility>
#include <unordered_map>

using std::unordered_map;
using std::vector;

// @lc code=start
class Solution
{
public:
    // 如果使用ordered_map, 会有额外的开销, 一般动态规划使用数组就行了
    vector<int> hashMap;
    int climbStairs(int n)
    {
        hashMap.resize(n + 1, -1);
        return helper(n);
    }

    int helper(int n)
    {
        if (n == 1)
            return 1;
        if (n == 2)
            return 2;
        if (hashMap[n] != -1)
            return hashMap[n];
        return hashMap[n] = helper(n - 1) + helper(n - 2);
    }
};
// @lc code=end

// @lc code=start
class Solution
{
public:
    unordered_map<int, int> hashMap;
    int climbStairs(int n)
    {
        if (n == 1)
            return 1;
        if (n == 2)
            return 2;
        auto pair = hashMap.find(n);
        if (pair != hashMap.end())
            return pair->second;
        else
        {
            int result = climbStairs(n - 1) + climbStairs(n - 2);
            hashMap.insert(std::make_pair(n, result));
            return result;
        }
    }
};
// @lc code=end

// @lc code=start
class Solution
{
public:
    // 纯递归实现, 会爆栈, N=45的时候能算对, 但是爆栈了会出错
    //                     f(6)
    //          f(4)                   f(5)
    //    f(2)       f(3)        f(3)       f(4)
    //            f(1) f(2)   f(1) f(2)  f(2) f(3)
    //                                     f(1) f(2)
    // 求f(5)的时候重复计算了f(4), 因此存在大量重复计算
    // 利用一个表存储先前计算的结果
    int climbStairs(int n)
    {
        if (n == 1)
            return 1;
        if (n == 2)
            return 2;

        return climbStairs(n - 1) + climbStairs(n - 2);
    }
};
// @lc code=end
