/*
 * @lc app=leetcode.cn id=234 lang=cpp
 *
 * [234] 回文链表
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
    // 栈解法, 栈默认一个元素必须出现两次, 所以没有办法处理奇数个元素
    bool isPalindrome1(ListNode *head)
    {
        if (head == nullptr)
            return true;
        if (head != nullptr && head->next == nullptr)
            return true;
        stack<int> s;
        while (head != nullptr)
        {
            if (s.empty() || s.top() != head->val)
                s.push(head->val);
            else
                s.pop();

            head = head->next;
        }
        return s.empty();
    }

public:
    // 快慢指针解法, 核心是反转链表
    bool isPalindrome(ListNode *head)
    {
        ListNode *fast = head, *slow = head;
        while (fast != nullptr && fast->next != nullptr)
        {
            fast = fast->next->next;
            slow = slow->next;
        }
        // 奇数的时候快指针不指向null
        if (fast != nullptr)
            slow = slow->next;
        // 反转链表之后比较
        fast = head;
        slow = reverseList(slow);
        while (slow != nullptr)
        {
            if (slow->val != fast->val)
                return false;
            fast = fast->next;
            slow = slow->next;
        }
        return true;
    }

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
// @lc code=end

class Solution
{
public:
    // 快慢指针解法, 核心是反转链表
    bool isPalindrome(ListNode *head)
    {
        ListNode *fast = head, *slow = head;
        while (fast != nullptr && fast->next != nullptr)
        {
            fast = fast->next->next;
            slow = slow->next;
        }
        // 奇数的时候快指针不指向null
        if (fast != nullptr)
            slow = slow->next;
        // 反转链表之后比较
        fast = head;
        slow = reverseList(slow);
        while (slow != nullptr)
        {
            if (slow->val != fast->val)
                return false;
            fast = fast->next;
            slow = slow->next;
        }
        return true;
    }

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