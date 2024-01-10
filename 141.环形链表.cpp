/*
 * @lc app=leetcode.cn id=141 lang=cpp
 *
 * [141] 环形链表
 */

#include <utility>
#include <unordered_map>

using std::unordered_map;

struct ListNode
{
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// @lc code=start
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
class Solution
{
public:
    // 快慢指针
    bool hasCycle(ListNode *head)
    {
        if (head == nullptr)
            return false;
        if (head == head->next)
            return true;
        ListNode *fast = head, *slow = head;
        while (fast->next != nullptr && fast->next->next != nullptr)
        {
            slow = slow->next;
            fast = fast->next->next;
            if (slow == fast)
                return true;
        }
        return false;
    }
};
// @lc code=end

class Solution
{
    // 哈希表, 时间复杂度O(N), 空间复杂度O(N)
    bool hasCycle(ListNode *head)
    {
        if (head == nullptr)
            return false;
        unordered_map<ListNode *, int> map;
        while (head != nullptr)
        {
            if (map.find(head) != map.end())
                return true;
            map.insert(std::make_pair(head, 1));
            head = head->next;
        }
        return false;
    }
};