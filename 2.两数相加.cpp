/*
 * @lc app=leetcode.cn id=2 lang=cpp
 *
 * [2] 两数相加
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
    ListNode *addTwoNumbers(ListNode *l1, ListNode *l2)
    {
        ListNode result;
        ListNode *cur = &result;

        int total = 0, next = 0;
        while (l1 != nullptr && l2 != nullptr)
        {
            total = l1->val + l2->val + next;
            next = total / 10;
            cur->next = new ListNode(total % 10);
            cur = cur->next;
            l1 = l1->next;
            l2 = l2->next;
        }
        while (l1 != nullptr)
        {
            total = l1->val + next;
            next = total / 10;
            cur->next = new ListNode(total % 10);
            cur = cur->next;
            l1 = l1->next;
        }
        while (l2 != nullptr)
        {
            total = l2->val + next;
            next = total / 10;
            cur->next = new ListNode(total % 10);
            cur = cur->next;
            l2 = l2->next;
        }
        if (next != 0)
            cur->next = new ListNode(next);
        return result.next;
    }
};
// @lc code=end
