/*
 * @lc app=leetcode.cn id=21 lang=cpp
 *
 * [21] 合并两个有序链表
 */

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
    // 时间复杂度: O(M+N), 空间复杂度: O(1)
    ListNode *mergeTwoLists(ListNode *list1, ListNode *list2)
    {
        if (list1 == nullptr)
            return list2;
        if (list2 == nullptr)
            return list1;
        ListNode result, *cur = &result, **which;
        while (list1 != nullptr && list2 != nullptr)
        {
            which = list1->val < list2->val ? &list1 : &list2;
            cur->next = *which;
            *which = (*which)->next;
            cur = cur->next;
        }
        if (list1 != nullptr)
            cur->next = list1;
        else
            cur->next = list2;
        return result.next;
    }
};
// @lc code=end

class Solution
{
    // 递归版本, 时间复杂度: O(M+N), 空间复杂度: O(M+N)
    void merger(ListNode *cur, ListNode *list1, ListNode *list2)
    {
        if (list1 == nullptr)
            cur->next = list2;
        else if (list2 == nullptr)
            cur->next = list1;
        else
        {
            ListNode **which = list1->val < list2->val ? &list1 : &list2;
            cur->next = *which;
            *which = (*which)->next;
            merger(cur->next, list1, list2);
        }
    }

    ListNode *mergeTwoLists(ListNode *list1, ListNode *list2)
    {
        ListNode result;
        merger(&result, list1, list2);
        return result.next;
    }
};