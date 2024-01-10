/*
 * @lc app=leetcode.cn id=1 lang=cpp
 *
 * [1] 两数之和
 */

#include <map>
#include <vector>
#include <iostream>
#include <unordered_map>

using std::map;
using std::unordered_map;
using std::vector;

// @lc code=start
class Solution
{
public:
    // brute-force, O(N^2)
    vector<int> twoSum(vector<int> &nums, int target)
    {
        bool found = false;
        size_t first = 0, second = 0;
        for (first = 0; first < nums.size(); first++)
        {
            for (second = first + 1; second < nums.size(); second++)
            {
                if (nums[second] == target - nums[first])
                {
                    found = true;
                    break;
                }
            }
            if (found)
                break;
        }
        return {(int)first, (int)second};
    }
};
// @lc code=end

// @lc code=start
class Solution
{
public:
    // Hashtable, O(2N)
    vector<int> twoSum(vector<int> &nums, int target)
    {
        map<int, int> hashTable;
        for (int i = 0; i < nums.size(); i++)
            // hashTable.insert(std::make_pair(nums[i], i));
            hashTable[nums[i]] = i;
        for (auto item = hashTable.begin(); item != hashTable.end(); item++)
        {
            auto second = hashTable.find(target - item->second);
            if (second != hashTable.end())
                return {item->first, second->second};
        }
    }
};
// @lc code=end

// @lc code=start
class Solution
{
public:
    // Hashtable by unordered_map, O(2N)
    // std::map is implemented as red-black tree, which has overheads when inserting
    // so use std::unordered_map which is implemented as true hash hashTable
    // this is faster
    // also use early stop
    vector<int> twoSum(vector<int> &nums, int target)
    {
        unordered_map<int, int> indices;
        for (int i = 0; i < nums.size(); i++)
        {
            if (indices.find(target - nums[i]) != indices.end())
                return {indices[target - nums[i]], i};
            indices[nums[i]] = i;
        }
        return {};
    }
};
// @lc code=end