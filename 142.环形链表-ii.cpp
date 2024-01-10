/*
 * @lc app=leetcode.cn id=142 lang=cpp
 *
 * [142] 环形链表 II
 */

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
    // 假设 环前x个节点, 环y个节点.
    // 则slow进入环的时候, fast已经走了2x个节点, 此时 fast 处于 x%y 处
    // 两者相遇时, slow 在 y - x%y处
    // 而后 slow 从头开始走 x, fast 也走 x
    // 则 再次相遇时, fast在 y - x%y + x处
    ListNode *detectCycle(ListNode *head)
    {
        if (head == nullptr)
            return nullptr;
        if (head == head->next)
            return head;
        bool hasCycle = false;
        ListNode *fast = head,
                 *slow = head;
        while (fast->next != nullptr && fast->next->next != nullptr)
        {
            slow = slow->next;
            fast = fast->next->next;
            if (slow == fast)
            {
                hasCycle = true;
                break;
            }
        }
        if (hasCycle)
        {
            slow = head;
            while (slow != fast)
            {
                slow = slow->next;
                fast = fast->next;
            }
            return slow;
        }
        return nullptr;
    }
};
// @lc code=end
