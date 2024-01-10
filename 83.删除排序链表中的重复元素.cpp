/*
 * @lc app=leetcode.cn id=83 lang=cpp
 *
 * [83] 删除排序链表中的重复元素
 */

#include <stack>

using std::stack;

struct ListNode
{
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// @lc code=start
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution
{
public:
    ListNode *deleteDuplicates(ListNode *head)
    {
        if (head == nullptr)
            return head;
        ListNode *ans = head, *cur = ans;
        int lastVal = ans->val;
        while (head != nullptr)
        {
            if (head->val != lastVal)
            {
                cur->next = head;
                lastVal = head->val;
                cur = cur->next;
            }
            head = head->next;
        }
        cur->next = nullptr;
        return ans;
    }
};
// @lc code=end

class Solution
{
public:
    // Simpler way
    ListNode *deleteDuplicates(ListNode *head)
    {
        if (head == nullptr)
            return head;
        ListNode *curr = head;
        while (curr->next != nullptr)
        {
            if (curr->next->val == curr->val)
                curr->next = curr->next->next;
            else
                curr = curr->next;
        }
        return head;
    }
};

class Solution
{
    // 递归版
    ListNode *deleteDuplicates(ListNode *head)
    {
        if (head == nullptr || head->next == nullptr)
            return head;
        head->next = deleteDuplicates(head->next);
        return head->val == head->next->val ? head->next : head;
    }
};