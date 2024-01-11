/*
 * @lc app=leetcode.cn id=206 lang=cpp
 *
 * [206] 反转链表
 */

#include <utility>

using std::pair;

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
    // 递归解法
    // newHead, last
    pair<ListNode *, ListNode *> reverser(ListNode *head)
    {
        // 边界条件
        if (head == nullptr)
            return {nullptr, nullptr};
        // 一个节点
        if (head->next == nullptr)
            return {head, head};
        // 两个节点
        if (head->next != nullptr && head->next->next == nullptr)
        {
            ListNode *newHead = head->next;
            ListNode *newLast = head;
            newHead->next = newLast;
            newLast->next = nullptr;
            return {newHead, newLast};
        }
        // 多个节点
        auto [newHead, newLast] = reverser(head->next);
        newLast->next = head;
        head->next = nullptr;
        return {newHead, head};
    }

    ListNode *reverseList(ListNode *head)
    {
        auto [newHead, last] = reverser(head);
        return newHead;
    }
};
// @lc code=end

class Solution
{
public:
    // 迭代解法
    ListNode *reverseList(ListNode *head)
    {
        ListNode *prev = nullptr;
        ListNode *curr = head;
        while (curr != nullptr)
        {
            ListNode *next = curr->next;
            curr->next = prev;
            prev = curr;
            curr = next;
        }
        return prev;
    }
};