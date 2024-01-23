#include <algorithm>
#include <deque>
#include <iostream>
#include <stack>
#include <utility>
#include <vector>

using std::cout;
using std::deque;
using std::pair;
using std::shared_ptr;
using std::stack;
using std::unique_ptr;
using std::vector;

template<typename T, typename U>
std::ostream &operator<<(std::ostream &os, const pair<T, U> &pair) {
	cout << "{" << pair.first << "," << pair.second << "}";
	return os;
}

template<typename T>
std::ostream &operator<<(std::ostream &os, const vector<T> &vec) {
	os << "{ ";
	for (size_t i = 0; i < vec.size(); i++) {
		os << vec[i] << (i == vec.size() - 1 ? " " : ", ");
	}
	os << "}";
	return os;
}


class SlidingWindow {
public:
	// 由一个代表题目，引出一种结构: 双端队列
	// 【题目】
	// 有一个整型数组arr和一个大小为w的窗口从数组的最左边滑到最右边，窗口每次向右边滑一个位置。
	// 例如，数组为[4,3,5,4,3,3,6,7]，窗口大小为3时:
	//      [435]43367
	//      4[354]3367
	//      43[543]367
	//      435[433]67
	//      4354[336]7
	//      43543[367]
	// 窗口中最大值分别为5, 5, 5, 6, 7
	// 如果数组长度为n，窗口大小为w，则一共产生n-w+1个窗口的最大值。
	// 请实现一个函数,
	// 输入: 整型数组arr，窗口大小为w。
	// 输出:一个长度为n-w+1的数组res，res[i]表示每一种窗口状态下的以本题为例，结果应该返回{5,5,5,4,6,7}。
	// = 双端队列就是可以在队列头尾入队列出队列的队列
	// = 根据不同的题目进行不同的维护, 这里的维护就是每次在队列尾入队列的时候, 如果队列尾元素不比当前要入队的元素大, 那么就出队列
	// 例如:
	// 队列为 [4, 3], 则当5要入队的时候, 弹出4和3, 最后5入队, 得到队列为 [5]
	// 如果队列尾 [4, 3, 2], 则当3要入队列, 弹出3和2, 最后新来的3入队, 得到队列为 [4, 3]
	// 针对这个题, 左侧边界动的时候就比较一下队列头的元素和左侧边界元素是否相等, 如果相等了那么就出队列
	// 这个队列头部维护的信息其实就是当前窗口下可能的最大值, 因为如果现在队列是 [4, 3], 新来了一个5
	// 那么不好意思, 在当前的窗口下, 4和3永远不可能有机会成为最大值了, 所以你们就出队吧
	static vector<int> slidingMax(const vector<int> &vec, int windowSize) {

		vector<int> result;
		deque<int> helpDeque;
		for (int rightIdx = 0, leftIdx = -windowSize; rightIdx < vec.size(); leftIdx++, rightIdx++) {
			while (!helpDeque.empty() && vec[rightIdx] >= vec[helpDeque.back()])
				helpDeque.pop_back();
			helpDeque.push_back(rightIdx);
			if (leftIdx >= 0)
				result.push_back(vec[helpDeque.front()]);
			if (leftIdx >= 0 && vec[leftIdx] == vec[helpDeque.front()])
				helpDeque.pop_front();
		}
		return std::move(result);
	}
};

class MonotoneStack {
public:
	// = 单调栈是递增单调栈或者递减单调栈, 对应能够找到数组中比这次入栈的元素小或者大的两边的元素
	static vector<pair<int, int>> getNearLessNoRepeat(vector<int> &nums) {
		stack<int> monoStack;
		vector<pair<int, int>> result(nums.size(), {-1, -1});
		for (int i = 0; i < nums.size(); i++) {
			while (!monoStack.empty() && nums[i] > nums[monoStack.top()]) {
				int curr = monoStack.top();
				monoStack.pop();
				int leftLessIndex = monoStack.empty() ? -1 : monoStack.top();
				result[curr].first = leftLessIndex;
				result[curr].second = i;
			}
			monoStack.push(i);
		}
		while (!monoStack.empty()) {
			int curr = monoStack.top();
			monoStack.pop();
			int leftLessIndex = monoStack.empty() ? -1 : monoStack.top();
			result[curr].first = leftLessIndex;
			result[curr].second = -1;
		}
		return std::move(result);
	}

	static vector<pair<int, int>> getNearLessWithRepeat(vector<int> &nums) {
		stack<shared_ptr<vector<int>>> monoStack;
		vector<pair<int, int>> result(nums.size(), {-1, -1});
		for (int i = 0; i < nums.size(); i++) {
			// 依次弹出
			while (!monoStack.empty() && nums[i] > nums[monoStack.top()->back()]) {
				shared_ptr<vector<int>> curr = monoStack.top();
				monoStack.pop();
				int leftLessIndex = monoStack.empty() ? -1 : monoStack.top()->back();
				for (int sameNumIdx: (*curr)) {
					result[sameNumIdx].first = leftLessIndex;
					result[sameNumIdx].second = i;
				}
			}
			// 值相同加入到链表中
			if (!monoStack.empty() && nums[monoStack.top()->back()] == nums[i])
				monoStack.top()->push_back(i);
			else
				monoStack.push(std::make_shared<vector<int>>(1, i));
		}

		// 收尾
		while (!monoStack.empty()) {
			shared_ptr<vector<int>> curr = monoStack.top();
			monoStack.pop();
			int leftLessIndex = monoStack.empty() ? -1 : monoStack.top()->back();
			for (int sameNumIdx: (*curr)) {
				result[sameNumIdx].first = leftLessIndex;
				result[sameNumIdx].second = -1;
			}
		}

		return std::move(result);
	}

	// 下面这问题就是基于上面的问题进行了进一步的演化
	// = 这节课讲的滑动窗口, 单调栈和双端队列, 以及前面的课讲的manacher算法, kmp算法和并查集
	// = 到底那些题的最优解法是基于这些解法改出来的?
	// = 这需要题目的敏感性, 大多数时候我们只需要在给定的问题的基础上进行化简, 就会发现最终的最优解题模板, 就是基于这些算法的
	// = 所以敏感性的建立, 就是需要去大量刷题才能够建立的, 报左神的中级班和提高班带着刷大量的题确实能够建立一定的敏感性
	// = 但是最终是需要去刷题, 才知道一个题在什么时候需要使用什么样的数据结构才能做的最好
	// 定义：非负数组中累积和与最小值的乘积，假设叫做指标A。给定一个非负数组，请返回子数组中，指标A最大的值
	// 问题建模: 假设给定 [5, 3, 2, 1, 6, 7, 8, 4]
	// 那么问题的解就是原数组中以每个数字作为最小值的的子数组的最大指标的最大值
	// 例如以5作为最小值的子数组得到一个指标最大值A, 以3作为最小值的子数组得到一个指标最大值B, ...
	// 那么指标A, B, C, D, E, F, G, H加在一起就涵盖了所有的子数组的可能性, 此时A~H取最大, 就得到了子数组中指标A的最大值
	// 建模过程就是
	// 以5作为最小值的子数组中哪一个指标最大? 只有一个[5], 因为如果往右扩了, 那么[5, 3]中5就不是最小的数了
	// 同理, 以3作为最小值的子数组中, [5, 3]的指标最大, 因为[5, 3, 2]中3就不是最小的了
	// 所以问题就化简为针对每一个数, 找到这个数左右两侧比自己小的数, 中间的值累加求和然后再乘以这个数就是以这个数为最小值的子数组中最大的指标的值
	// = 所以这个问题就是每次要找到某个数左右两边比自己小的数, 对应就是递增单调栈
	static int maxSubArray(vector<int> &nums) {
		// 提前计算好累加和, 这样方便后面加
		vector<int> accumulation{nums[0]};
		for (int i = 1; i < nums.size(); i++)
			accumulation.push_back(accumulation[i - 1] + nums[i]);

		int maxValue = 0;
		stack<int> increaseStack;
		// 全都入栈
		for (int i = 0; i < nums.size(); i++) {
			while (!increaseStack.empty() && nums[i] <= nums[increaseStack.top()]) {
				int lessThanCurrIndex = increaseStack.top();
				increaseStack.pop();
				maxValue = std::max(maxValue, (increaseStack.empty() ? accumulation[i - 1] : (accumulation[i - 1] - accumulation[increaseStack.top()])) * nums[lessThanCurrIndex]);
			}
			increaseStack.push(i);
		}

		// 清理栈
		while (!increaseStack.empty()) {
			int lessThanCurrIndex = increaseStack.top();
			increaseStack.pop();
			maxValue = std::max(maxValue, (increaseStack.empty() ? accumulation[nums.size() - 1] : (accumulation[nums.size() - 1] - accumulation[increaseStack.top()])) * nums[lessThanCurrIndex]);
		}

		return maxValue;
	}
};


int main(int argc, char *argv[]) {
	vector<int> nums{4, 3, 5, 4, 3, 3, 6, 7};
	cout << "Sliding Window: " << SlidingWindow::slidingMax(nums, 3) << "\n";

	vector<int> numsNoRepeat{5, 4, 3, 6, 1, 2, 0, 7};
	vector<int> numsWithRepeat{5, 4, 3, 4, 5, 6, 5, 6};
	vector<int> maxSubArray{5, 3, 2, 1, 6, 7, 8, 4};
	cout << "getNearLessNoRepeat: " << MonotoneStack::getNearLessNoRepeat(numsNoRepeat) << "\n";
	cout << "getNearLessWithRepeat: " << MonotoneStack::getNearLessWithRepeat(numsWithRepeat) << "\n";
	cout << "maxSubArray: " << MonotoneStack::maxSubArray(maxSubArray) << "\n";
	return 0;
}
