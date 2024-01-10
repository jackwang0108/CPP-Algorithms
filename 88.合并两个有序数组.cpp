/*
 * @lc app=leetcode.cn id=88 lang=cpp
 *
 * [88] 合并两个有序数组
 */

#include <vector>

using std::vector;

// @lc code=start
class Solution
{
public:
    // 时间复杂度: O(2(M+N)), 空间复杂度: O((M+N))
    void merge(vector<int> &nums1, int m, vector<int> &nums2, int n)
    {
        int nums1Ptr = 0, nums2Ptr = 0, resultPtr = 0;
        vector<int> result = vector<int>(m + n);
        while (nums1Ptr < m && nums2Ptr < n)
        {
            if (nums1[nums1Ptr] < nums2[nums2Ptr])
                result[resultPtr++] = nums1[nums1Ptr++];
            else
                result[resultPtr++] = nums2[nums2Ptr++];
        }
        while (nums1Ptr < m)
            result[resultPtr++] = nums1[nums1Ptr++];
        while (nums2Ptr < n)
            result[resultPtr++] = nums2[nums2Ptr++];
        for (int i = 0; i < m + n; i++)
            nums1[i] = result[i];
    }
};
// @lc code=end

// @lc code=start
class Solution
{
public:
    // 利用排列, 时间复杂度O(M+N), 空间复杂度O(1)
    void merge(vector<int> &nums1, int m, vector<int> &nums2, int n)
    {
        int nums1Ptr = m - 1, nums2Ptr = n - 1, resultPtr = m + n - 1;
        while (0 <= nums1Ptr && 0 <= nums2Ptr)
        {
            if (nums1[nums1Ptr] > nums2[nums2Ptr])
                nums1[resultPtr--] = nums1[nums1Ptr--];
            else
                nums1[resultPtr--] = nums2[nums2Ptr--];
        }
        while (0 <= nums1Ptr)
            nums1[resultPtr--] = nums1[nums1Ptr--];
        while (0 <= nums2Ptr)
            nums1[resultPtr--] = nums2[nums2Ptr--];
    }
};
