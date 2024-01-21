#include <algorithm>
#include <iostream>
#include <map>
#include <queue>
#include <random>
#include <set>
#include <stack>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using std::cout;
using std::endl;
using std::map;
using std::queue;
using std::set;
using std::stack;
using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;

template<typename T>
std::ostream &operator<<(std::ostream &os, vector<T> &vec) {
	for (auto &v: vec)
		os << v << " ";
	return os;
}

class UseSets {
public:
	// 哈希表的简单介绍
	// 1）哈希表在使用层面上可以理解为一种集合结构
	// 2）如果只有key，没有伴随数据value，可以使用HashSet结构(C++中叫UnOrderedSet)
	// 3）如果既有key，又有伴随数据value，可以使用HashMap结构(C++中叫UnOrderedMap)
	// 4）有无伴随数据，是HashMap和HashSet唯一的区别，底层的实际结构是一回事
	// =
	// 5）使用哈希表增(put)、删(remove)、改(put)和查(get)的操作，可以认为时间复杂度为O(1)，但是常数时间比较大
	// = 6）放入哈希表的东西，如果是基础类型
	// (基础类型或者引用)，内部按值传递，内存占用就是这个东西的大小 =
	// 7）放入哈希表的东西，如果不是基础类型
	// (定义的类)，内部按引用传递，内存占用是这个东西内存地址的大小有关
	// 哈希表的原理，将在提升班“与哈希函数有关的数据结构”一章中讲叙原理题目一
	static void useHashSet() {
		// 哈希表底层的实现就是使用哈希函数实现散列映射
		unordered_set<int> hashSet;
		// 增加操作
		hashSet.insert(3);
		hashSet.insert(4);
		// 查询操作
		cout << "hashSet contains 3? " << hashSet.contains(3) << endl;
		cout << "hashSet contains 4? " << hashSet.contains(4) << endl;
		cout << "hashSet contains 5? " << hashSet.contains(5) << endl;
		// 查询元素的位置
		cout << "hashSet 4: " << *hashSet.find(4) << endl;
		// 删除操作
		hashSet.erase(3);
		cout << "hashSet contains 3? " << hashSet.contains(3) << endl;
		// unordered_set中的元素是不可修改的，因为元素的值用于计算哈希值，修改元素可能会导致哈希值的变化，破坏哈希表的结构。如果需要修改元素，需要先删除旧元素，然后插入新元素。

		// 有无伴随数据，是HashMap和HashSet唯一的区别，底层的实际结构是一回事
		// 对于C++, unordered_map实际上就是unordered_set,
		// 只不过每一个元素变成了std::pair
		unordered_map<int, string> hashMap;
		// 增加操作
		hashMap.insert(std::make_pair(1, "wang"));
		hashMap.insert(std::make_pair(2, "shi"));
		hashMap.insert(std::make_pair(3, "hong"));
		// 查询操作
		cout << "hashMap contains 1? " << hashMap.contains(1) << endl;
		cout << "hashMap contains 2? " << hashMap.contains(2) << endl;
		cout << "hashMap contains 4? " << hashMap.contains(4) << endl;
		// 查找元素的位置
		cout << "hashMap key 1: " << hashMap.find(1)->second << endl;
		// 删除操作
		hashMap.erase(1);
		cout << "hashMap contains 1? " << hashMap.contains(1) << endl;
		// 修改操作
		hashMap[2] = "hong";
		hashMap.insert_or_assign(3, "shen");
		cout << "hashMap key 2: " << hashMap.find(2)->second << endl;
		cout << "hashMap key 3: " << hashMap.find(3)->second << endl;
	}

	// 有序表的简单介绍
	// 1）有序表在使用层面上可以理解为一种集合结构
	// 2）如果只有key，没有伴随数据value，可以使用TreeSet结构(C++中叫OrderedSet)
	// 3）如果既有key，又有伴随数据value，可以使用TreeMap结构(C++中叫OrderedMap)
	// = 4）有无伴随数据，是TreeSet和TreeMap唯一的区别，底层的实际结构是一回事
	// 5）有序表和哈希表的区别是，有序表把key按照顺序组织起来，而哈希表完全不组织5）红黑树、AVL树、size-balance-tree和跳表等都属于有序表结构，只是底层具体实现不同
	// 6）放入哈希表的东西，如果是基础类型，内部按值传递，内存占用就是这个东西的大小
	// 7）放入哈希表的东西，如果不是基础类型，必须提供比较器，内部按引用传递，内存占用是这个东西内存地址的大小
	// 8）不管是什么底层具体实现，只要是有序表，都有以下固定的基本功能和固定的时间复杂度
	static void useTreeSet() {
		// 有序表底层的实现是使用树实现的
		set<int> orderedSet;
		// 增加操作
		orderedSet.insert(3);
		orderedSet.insert(4);
		orderedSet.insert(5);
		// 查询操作
		cout << "orderedSet contains 3? " << orderedSet.contains(3) << endl;
		cout << "orderedSet contains 4? " << orderedSet.contains(4) << endl;
		cout << "orderedSet contains 6? " << orderedSet.contains(6) << endl;
		// 查询元素的位置
		cout << "orderedSet 4: " << *orderedSet.find(4) << endl;
		// 删除操作
		orderedSet.erase(4);
		cout << "orderedSet contains 4? " << orderedSet.contains(4) << endl;
		// 修改操作
		// std::set中的元素是只读的，无法直接修改。如果需要修改元素，需要先删除旧元素，然后插入新元素

		// 有无伴随数据，是orderedSet和orderedMap唯一的区别，底层的实际结构是一回事
		// 对于C++, map实际上就是set, 只不过每一个元素变成了std::pair
		map<int, string> orderedMap;
		// 增加操作
		orderedMap.insert(std::make_pair(1, "wang"));
		orderedMap.insert(std::make_pair(2, "shi"));
		orderedMap.insert(std::make_pair(3, "hong"));
		// 查询操作
		cout << "orderedMap contains 1? " << orderedMap.contains(1) << endl;
		cout << "orderedMap contains 2? " << orderedMap.contains(2) << endl;
		cout << "orderedMap contains 4? " << orderedMap.contains(4) << endl;
		// 查找元素的位置
		cout << "orderedMap key 1: " << orderedMap.find(1)->second << endl;
		// 删除操作
		orderedMap.erase(1);
		cout << "orderedMap contains 1? " << orderedMap.contains(1) << endl;
		// 修改操作
		orderedMap[2] = "hong";
		orderedMap.insert_or_assign(3, "shen");
		cout << "orderedMap key 2: " << orderedMap.find(2)->second << endl;
		cout << "orderedMap key 3: " << orderedMap.find(3)->second << endl;
	}
};

// 单链表
template<typename T>
struct SingleNode {
	T val;
	SingleNode *next;
	SingleNode *rand;

	SingleNode()
	    : next(nullptr), rand(nullptr){};

	explicit SingleNode(T val)
	    : val(val), next(nullptr), rand(nullptr){};

	explicit SingleNode(vector<T> &&vec)
	    : val(0), next(nullptr), rand(nullptr) {
		SingleNode *prev = this, *curr;
		for (auto i: vec) {
			curr = new SingleNode<T>;
			curr->val = i;
			prev->next = curr;
			prev = curr;
		}
	}

	explicit SingleNode(vector<T> &vec)
	    : val(0), next(nullptr) {
		SingleNode *prev = this, *curr;
		for (auto i: vec) {
			curr = new SingleNode<T>;
			curr->val = i;
			prev->next = curr;
			prev = curr;
		}
	}

	static SingleNode<T> *generate(int nodeNum) {
		std::random_device rd; // 用于获取随机种子
		std::mt19937 gen(rd());// 使用 Mersenne Twister 引擎，种子为 rd()
		std::uniform_int_distribution<int> dis(
		    0,
		    20);// 定义整数分布范围为 [min, max]

		auto *head = new SingleNode<T>;
		SingleNode<T> *curr = head;
		while (nodeNum-- > 0) {
			curr->next = new SingleNode<T>;
			curr->next->val = dis(gen);
			curr = curr->next;
		}
		return head;
	}

	static void release(SingleNode<T> *head) {
		SingleNode<T> *next;
		while (head != nullptr) {
			next = head->next;
			delete head;
			head = next;
		}
	}
};

// 双链表
template<typename T>
struct DoubleNode {
	T val;
	DoubleNode *next;
	DoubleNode *prev;

	DoubleNode()
	    : val(0), next(nullptr), prev(nullptr) {
	}

	explicit DoubleNode(T val)
	    : val(val), next(nullptr), prev(nullptr) {
	}

	explicit DoubleNode(vector<T> &&vec)
	    : val(0), next(nullptr), prev(nullptr) {
		DoubleNode *curr, *previous = this;
		for (auto i: vec) {
			curr = new DoubleNode<int>;
			curr->val = i;
			curr->prev = previous;
			curr->prev->next = curr;
			previous = curr;
			curr = curr->next;
		}
	}

	explicit DoubleNode(vector<T> &vec)
	    : val(0), next(nullptr), prev(nullptr) {
		DoubleNode *curr, *previous = this;
		for (auto i: vec) {
			curr = new DoubleNode<int>;
			curr->val = i;
			curr->prev = previous;
			curr->prev->next = curr;
			previous = curr;
			curr = curr->next;
		}
	}

	static DoubleNode<T> *generate(int nodeNum) {
		std::random_device rd; // 用于获取随机种子
		std::mt19937 gen(rd());// 使用 Mersenne Twister 引擎，种子为 rd()
		std::uniform_int_distribution<int> dis(
		    0,
		    20);// 定义整数分布范围为 [min, max]

		auto *head = new DoubleNode<T>;
		DoubleNode<T> *curr = head;
		while (nodeNum-- > 0) {
			curr->next = new DoubleNode<T>;
			curr->next->val = dis(gen);
			curr->next->prev = curr;
			curr = curr->next;
		}
		return head;
	}

	static void release(DoubleNode<T> *head) {
		DoubleNode<T> *next;
		while (head != nullptr) {
			next = head->next;
			delete head;
			head = next;
		}
	}
};

// 元编程
template<typename NodeType>
struct IsSingleNode {
	static constexpr bool value = false;
};

template<typename T>
struct IsSingleNode<SingleNode<T>> {
	static constexpr bool value = true;
};

template<typename NodeType>
struct IsDoubleNode {
	static constexpr bool value = false;
};

template<typename T>
struct IsDoubleNode<DoubleNode<T>> {
	static constexpr bool value = true;
};

template<typename NodeType, typename T>
NodeType *print(NodeType *head) {
	string delim;
	if constexpr (std::is_same_v<SingleNode<T> *, decltype(head)>)
		delim = " -> ";
	else if constexpr (std::is_same_v<DoubleNode<T> *, decltype(head)>) {
		delim = " <-> ";
		cout << "NULL <-> ";
	}

	auto *curr = head;
	while (curr != nullptr)
		cout << curr->val << delim, curr = curr->next;
	cout << "NULL" << endl;
	return head;
}

class LinkedListProblems {
public:
	// 反转单链表
	template<typename T>
	static SingleNode<T> *reverseSingle(SingleNode<T> *head) {
		SingleNode<T> *curr = head, *prev = nullptr;
		while (curr != nullptr) {
			SingleNode<T> *next = curr->next;
			curr->next = prev;
			prev = curr;
			curr = next;
		}
		return prev;
	}

	// 反转双链表
	template<typename T>
	static DoubleNode<T> *reverseDouble(DoubleNode<T> *head) {
		DoubleNode<T> *curr = head, *temp = nullptr;
		while (curr != nullptr) {
			temp = curr->prev;
			curr->prev = curr->next;
			curr->next = temp;
			curr = curr->prev;
		}
		return temp->prev;
	}

	// 查找两个链表的公共部分
	template<typename T>
	static int findCommon(SingleNode<T> *head1, SingleNode<T> *head2) {
		int sum = 0;
		SingleNode<T> *first = head1, *second = head2, **which;
		while (first != nullptr && second != nullptr) {
			if (first->val == second->val)
				first = first->next, second = second->next, sum++;
			which = first->val < second->val ? &first : &second;
			*which = (*which)->next;
		}
		return sum;
	}

	// 面试时链表解题的方法论
	// 1）对于笔试，不用太在乎空间复杂度，一切为了时间复杂度
	// 2）对于面试，时间复杂度依然放在第一位，但是一定要找到空间最省的方法
	//
	// 重要技巧：
	// 1）额外数据结构记录（哈希表等）
	// 2）快慢指针
	template<typename T>
	static bool isPrimordial1(SingleNode<T> *head) {
		// 笔试做法, 不用考虑空间复杂度, 直接开一个栈
		stack<int> st;
		SingleNode<T> *copy = head;
		while (head != nullptr) {
			st.push(head->val);
			head = head->next;
		}
		while (!st.empty()) {
			if (copy->val != st.top())
				return false;
			st.pop();
			copy = copy->next;
		}
		return true;
	}

	template<typename T>
	static bool isPrimordial2(SingleNode<T> *head) {
		// 面试的时候要做到额外空间复杂度为O(1)
		// 找到一半的地方然后反转链表即可
		int length = 0;
		SingleNode<T> *copy = head;
		while (copy != nullptr)
			length++, copy = copy->next;

		SingleNode<T> *slow = head, *fast = head;
		for (int i = 0; i < (length + 1) / 2; i++)
			fast = fast->next;

		fast = reverseSingle(fast);
		while (slow != nullptr && fast != nullptr) {
			if (slow->val != fast->val)
				return false;
			slow = slow->next;
			fast = fast->next;
		}
		return true;
	}

	template<typename T>
	static SingleNode<T> *hollandFlag1(SingleNode<T> *head, T num) {
		// 笔试做法, 直接数组排序, 然后再串起来
		vector<SingleNode<T> *> vec;
		SingleNode<T> *curr = head;
		while (curr != nullptr) {
			vec.push_back(curr);
			curr = curr->next;
		}

		// 荷兰国旗
		int lessRegion = -1, greaterRegion = vec.size();
		for (int i = 0; i < greaterRegion;) {
			if (vec[i]->val < num)
				std::swap(vec[++lessRegion], vec[i]);
			else if (vec[i]->val > num)
				std::swap(vec[--greaterRegion], vec[i]);
			else
				i++;
		}

		// 串起来
		for (int i = 0; i < vec.size() - 1; i++)
			vec[i]->next = vec[i + 1];
		vec[vec.size() - 1]->next = nullptr;
		return vec[0];
	}

	template<typename T>
	static SingleNode<T> *hollandFlag2(SingleNode<T> *head, int num) {
		// 面试时的解法, 六个变量解决了
		SingleNode<T> *lessHead, *lessTail, *equalHead, *equalTail, *greaterHead, *greaterTail;
		lessHead = lessTail = equalHead = equalTail = greaterHead = greaterTail = nullptr;
		SingleNode<T> **whichHead, **whichTail;
		while (head != nullptr) {
			if (head->val < num)
				whichHead = &lessHead, whichTail = &lessTail;
			else if (head->val == num)
				whichHead = &equalHead, whichTail = &equalTail;
			else
				whichHead = &greaterHead, whichTail = &greaterTail;
			if (*whichHead == nullptr)
				*whichHead = *whichTail = head;
			else {
				(*whichTail)->next = head;
				(*whichTail) = (*whichTail)->next;
			}
			head = head->next;
		}
		// 边界条件需要讨论清楚, 如果没有小于和等于区域呢?
		// 如果有小于区域
		if (lessTail != nullptr) {
			lessTail->next = equalHead;
			// 谁连大于区域
			equalTail = equalTail == nullptr ? lessTail : equalTail;
		}
		// 如果有小于等于区域
		if (equalTail != nullptr) {
			equalTail->next = greaterHead;
		}
		return lessHead != nullptr ? lessHead : (equalHead != nullptr ? equalHead : greaterHead);
	}

	// 复制含有随机指针节点的链表
	// 【题目】一种特殊的单链表节点类描述如下
	// class Node
	// {
	//     int value;
	//     Node next;
	//     Node rand;
	//     Node(int val) { value = val; }
	// }
	// rand指针是单链表节点结构中新增的指针，rand可能指向链表中的任意一个节点，也可能指向null。
	// 给定一个由Node节点类型组成的无环单链表的头节点head，请实现一个函数完成这个链表的复制，并返回复制的新链表的头节点
	template<typename T>
	static SingleNode<T> *copyListWithRand1(SingleNode<T> *head) {
		// 笔试算法, 开一个map实现新老节点的一一对应
		unordered_map<SingleNode<T> *, SingleNode<T> *> nodeMap;
		SingleNode<T> *curr = head;
		// 复制元素
		while (curr != nullptr) {
			nodeMap.insert(std::make_pair(curr, new SingleNode<T>));
			curr = curr->next;
		}
		// 复制rand指针
		curr = head;
		while (curr != nullptr) {
			nodeMap[curr]->next = nodeMap[curr->next];
			nodeMap[curr]->rand = nodeMap[curr->rand];
			nodeMap[curr]->val = curr->val;
			curr = curr->next;
		}
		SingleNode<T> *newHead = new SingleNode<T>;
		newHead->next = nodeMap[head];
		return newHead;
	}

	template<typename T>
	static SingleNode<T> *copyListWithRand2(SingleNode<T> *head) {
		// 面试做法, 实现O(1)的空间复杂度
		// 核心是新老节点的一一映射, 所以使用链表的链接特性实现
		SingleNode<T> *curr = head, *next, *newCurr, *newHead, *newNext;
		// 复制元素
		while (curr != nullptr) {
			next = curr->next;
			curr->next = new SingleNode<T>;
			curr->next->val = curr->val;
			curr->next->next = next;
			curr = curr->next->next;
		}
		// 复制rand指针
		curr = head;
		while (curr != nullptr && curr->rand != nullptr) {
			curr->next->rand = curr->rand->next;
			curr = curr->next->next;
		}
		// 拆分两个链表
		curr = head, newHead = head->next, newCurr = newHead;
		while (curr != nullptr && curr->next != nullptr && curr->next->next != nullptr &&
		       curr->next->next->next != nullptr) {
			next = curr->next->next;
			newNext = next->next;
			curr->next = next;
			newCurr->next = newNext;
			curr = curr->next;
			newCurr = newCurr->next;
		}
		SingleNode<T> *newHeadh = new SingleNode<T>;
		newHeadh->next = newHead;
		return newHeadh;
	}

	template<typename T>
	static bool hasCircle1(SingleNode<T> *head) {
		// 笔试做法, 使用HashMap
		if (head == nullptr)
			return false;
		unordered_set<SingleNode<T> *> hashSet;
		while (head != nullptr) {
			if (hashSet.find(head) != hashSet.end())
				return true;
			hashSet.insert(head);
			head = head->next;
		}
		return false;
	}

	template<typename T>
	static bool hasCircle2(SingleNode<T> *head) {
		// 面试做法, 空间复杂度O(1)
		if (head->next == nullptr || head->next->next == nullptr)
			return false;
		SingleNode<T> *slow = head, *fast = head->next->next;
		while (slow != fast && fast != nullptr && fast->next != nullptr) {
			fast = fast->next->next;
			slow = slow->next;
		}
		return slow == fast;
	}

	template<typename T>
	static SingleNode<T> *findFirstNode(SingleNode<T> *head) {
		if (head->next == nullptr || head->next->next == nullptr)
			return nullptr;
		SingleNode<T> *slow = head->next, *fast = head->next->next;
		while (slow != fast && fast->next != nullptr && fast->next->next != nullptr)
			slow = slow->next, fast = fast->next->next;

		fast = head;
		while (fast != slow)
			fast = fast->next, slow = slow->next;

		return fast;
	}
};

int main(int argc, char *argv[]) {
	UseSets::useHashSet();
	UseSets::useTreeSet();

	// 生成单链表
	SingleNode<int> *testSingle1 = SingleNode<int>::generate(10);
	print<SingleNode<int>, int>(testSingle1);
	SingleNode<int> testSingle2 = SingleNode<int>(vector<int>{1, 2, 3});
	print<SingleNode<int>, int>(&testSingle2);

	// 生成双链表
	DoubleNode<int> *testDouble1 = DoubleNode<int>::generate(10);
	print<DoubleNode<int>, int>(testDouble1);
	DoubleNode<int> testDouble2 = DoubleNode<int>(vector<int>{1, 2, 3, 2, 1});
	print<DoubleNode<int>, int>(&testDouble2);

	// 反转单链表
	SingleNode<int> single1(vector<int>{1, 2, 3});
	print<SingleNode<int>, int>(&single1);
	print<SingleNode<int>, int>(LinkedListProblems::reverseSingle(&single1));

	// 反转双链表
	DoubleNode<int> double2(vector<int>{1, 2, 3, 4, 5});
	print<DoubleNode<int>, int>(&double2);
	print<DoubleNode<int>, int>(LinkedListProblems::reverseDouble(&double2));

	// 求公共部分
	SingleNode<int> first(vector<int>{1, 2, 3, 4});
	SingleNode<int> second(vector<int>{-1, -2, -3, 2, 3, 9});
	cout << "Common part: " << LinkedListProblems::findCommon(&first, &second)
	     << endl;

	// 判断是否为回文结构
	vector<int> isPrimordialVec{1, 2, 3, 4, 3, 2, 1};
	SingleNode<int> isPrimordial(isPrimordialVec);
	vector<int> nonPrimordialVec{1, 2, 3, 4, 5};
	SingleNode<int> nonPrimordial(nonPrimordialVec);
	cout << "linked list: " << isPrimordialVec << ", isPrimordial1: "
	     << LinkedListProblems::isPrimordial1(isPrimordial.next) << endl;
	cout << "linked list: " << nonPrimordialVec << ", isPrimordial1: "
	     << LinkedListProblems::isPrimordial1(nonPrimordial.next) << endl;
	cout << "linked list: " << isPrimordialVec << ", isPrimordial2: "
	     << LinkedListProblems::isPrimordial2(isPrimordial.next) << endl;
	cout << "linked list: " << nonPrimordialVec << ", isPrimordial2: "
	     << LinkedListProblems::isPrimordial2(nonPrimordial.next) << endl;

	//	荷兰国旗问题
	vector<int> hollandFlagVec1{5, 0, 2, 3, 4, 7, 8, 0, -1, -3, 5,
	                            9, 4, 6, 7, 8, 5, 5, 6, 7, 8, 9};
	SingleNode<int> hollandFlag1(hollandFlagVec1);
	print<SingleNode<int>, int>(LinkedListProblems::hollandFlag1(hollandFlag1.next, 5));
	vector<int> hollandFlagVec2{5, 0, 2, 3, 4, 7, 8, 0, -1, -3, 5,
	                            9, 4, 6, 7, 8, 5, 5, 6, 7, 8, 9};
	SingleNode<int> hollandFlag2(hollandFlagVec2);
	print<SingleNode<int>, int>(LinkedListProblems::hollandFlag2(hollandFlag2.next, 5));

	// 复制含有随机指针节点的链表
	SingleNode<int> *randLinkedList = SingleNode<int>::generate(5);
	randLinkedList->next->rand = randLinkedList->next->next->next;
	randLinkedList->next->next->rand = randLinkedList->next->next;
	print<SingleNode<int>, int>(randLinkedList);
	cout << "rand: " << randLinkedList->next->rand->val << ", " << randLinkedList->next->next->rand->val << endl;
	SingleNode<int> *copied1 = LinkedListProblems::copyListWithRand1(randLinkedList->next);
	print<SingleNode<int>, int>(copied1);
	cout << "rand: " << copied1->next->rand->val << ", " << copied1->next->next->rand->val << endl;
	SingleNode<int> *copied2 = LinkedListProblems::copyListWithRand2(randLinkedList->next);
	print<SingleNode<int>, int>(copied2);
	cout << "rand: " << copied2->next->rand->val << ", " << copied2->next->next->rand->val << endl;

	// 判断链表是否有环
	//	1 -> 2 -> 3 -> 4 -> 1
	SingleNode<int> *circleLinkedList = SingleNode<int>::generate(4);
	circleLinkedList->next->next->next->next->next = circleLinkedList->next;
	cout << std::boolalpha;
	cout << "hasCircle: " << LinkedListProblems::hasCircle1(circleLinkedList->next) << endl;
	cout << "hasCircle: " << LinkedListProblems::hasCircle2(circleLinkedList->next) << endl;
	SingleNode<int> *nonCircleLinkedList = SingleNode<int>::generate(4);
	cout << "hasCircle: " << LinkedListProblems::hasCircle1(nonCircleLinkedList) << endl;
	cout << "hasCircle: " << LinkedListProblems::hasCircle2(nonCircleLinkedList) << endl;

	// 获得第一个入环节点
	cout << "First Circle Node: " << LinkedListProblems::findFirstNode(circleLinkedList->next)->val << endl;
	return 0;
}
