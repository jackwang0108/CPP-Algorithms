#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <string>
#include <utility>
#include <vector>

using std::cout;
using std::map;
using std::pair;
using std::queue;
using std::set;
using std::shared_ptr;
using std::string;
using std::vector;

template<typename T>
std::ostream &operator<<(std::ostream &os, const vector<T> &vec) {
	os << "{";
	for (int i = 0; i < vec.size(); i++)
		cout << i << (i == vec.size() - 1 ? "}" : ", ");
	return os;
}

template<typename T, typename U>
std::ostream &operator<<(std::ostream &os, const pair<T, U> &p) {
	os << "{" << p.first << ", " << p.second << "}";
	return os;
}

struct TreeNode {
	int val = 0;
	shared_ptr<TreeNode> left;
	shared_ptr<TreeNode> right;

	TreeNode() {}
	~TreeNode() {}

	explicit TreeNode(int val) : val(val) {}
	explicit TreeNode(const vector<int> &vec) {
		if (vec.empty())
			return;

		val = vec[0];
		left = buildTree(vec, 1);
		right = buildTree(vec, 2);
	}

	shared_ptr<TreeNode> buildTree(const vector<int> &vec, int idx) {
		if (idx >= vec.size())
			return nullptr;
		shared_ptr<TreeNode> curr = std::make_shared<TreeNode>(vec[idx]);


		int leftChildIdx = idx * 2 + 1;
		int rightChildIdx = idx * 2 + 2;
		curr->left = buildTree(vec, leftChildIdx);
		curr->right = buildTree(vec, rightChildIdx);

		return std::move(curr);
	}

	friend std::ostream &operator<<(std::ostream &os, const TreeNode &node) {
		cout << node.val << " ";
		if (node.left)
			cout << *node.left;
		if (node.right)
			cout << *node.right;
		return os;
	}
};


class ChineseExpression {
	// 把一个数字用中文表示出来。数字范围为[0,99999]。
	// 为了方便输出，使用字母替换相应的中文，万W千Q百B十S零L。
	// 使用数字取代中文数字
	// 注：对于11应该表示为一十一(1S1)，而不是十一(S1)
	// 输入描述：数字0（包含）到99999（包含）。
	// 输出描述：用字母替换相应的中文，万W千Q百B十S零L
	// 示例1: 输入12001, 输出1W2QL1

public:
	// 纯coding题
	static string chineseExpression(int num) {
		if (num < 0 || num > 99999)
			return "";
		return process1To99999999(num);
	}

	static string process1To9(int num) {
		if (num < 1 || num > 9)
			return "";
		vector<string> map = {"一", "二", "三", "四", "五", "六", "七", "八", "九"};
		return map[num - 1];
	}

	static string process1To99(int num, bool hasBai) {
		if (num < 1 || num > 99)
			return "";
		if (num < 10)
			return process1To9(num);

		int shi = num / 10;
		if (shi == 1 && !hasBai)
			return "十" + process1To9(num % 10);
		else
			return process1To9(shi) + "十" + process1To9(num % 10);
	}

	static string process1To999(int num) {
		if (num < 1 || num > 999)
			return "";
		if (num < 100)
			return process1To99(num, false);

		string result = process1To9(num / 100) + "百";

		int rest = num % 100;
		if (rest == 0)
			return result;
		else if (rest < 10)
			return result + "零" + process1To9(rest);
		else
			return result + process1To99(rest, true);
	}

	static string process1To9999(int num) {
		if (num < 1 || num > 9999)
			return "";

		if (num < 1000)
			return process1To999(num);

		string result = process1To9(num / 1000) + "千";

		int rest = num % 1000;
		if (rest == 0)
			return result;
		else if (rest < 100)
			return result + "零" + process1To99(rest, false);
		else
			return result + process1To999(rest);
	}

	static string process1To99999999(int num) {
		if (num < 1 || num > 99999999)
			return "";

		string result = process1To9999(num / 10000) + "万";

		int rest = num % 10000;
		if (rest == 0)
			return result;
		else if (rest < 1000)
			return result + "零" + process1To999(rest);
		else
			return result + process1To9999(rest);
	}
};

class CompleteTreeNodeNumber {
	// 求完全二叉树节点的个数
public:
	// 遍历可以解决, 复杂度是O(N), 通过递归实现O(logN)级别的复杂度
	// 递归的核心思路就是 当前子树节点数 = 左子树节点数 + 1 + 右子树节点数
	// 左子树节点数取决左子树是否为满二叉树, 故看右子树最左节点的深度是否达到最深
	// 如果达到了最深, 那么左子树就是满二叉树, 然后递归计算右子树
	// 否则右子树一定是满二叉树, 然后递归计算左子树
	static int completeTreeNodeNumber(const vector<int> &vec) {
		if (vec.empty())
			return 0;
		shared_ptr<TreeNode> root = std::make_shared<TreeNode>(vec);
		return process(root, 1, mostLeftLevel(root, 1));
	}

	// 以在level层的root为根的子树的最大深度
	static int mostLeftLevel(shared_ptr<TreeNode> root, int level) {
		while (root != nullptr)
			level++, root = root->left;
		return level - 1;
	}

	static int process(shared_ptr<TreeNode> &root, int level, int depth) {
		// 最后一层
		if (level == depth)
			return 1;
		if (mostLeftLevel(root->right, level + 1) == depth)
			return (1 << (depth - level)) + process(root->right, level + 1, depth);
		else
			return (1 << (depth - level - 1)) + process(root->left, level + 1, depth);
	}
};

class SubsequenceProblem {
	// 最长递增子序列问题
	// 这里需要注意, 子数组必须是连续的, 而子序列则可以是不连续的
public:
	// 经典解法, dp
	// 针对子序列问题, dp表中维护的信息就是dp[i]表示以vec[i]结尾的子序列的长度
	// 所以这道题dp解法的话, dp[i]就是以vec[i]为结尾的最长递增子序列的长度
	// 而后dp[i]以来从dp[0]一直到dp[i-1]
	// 因此是从左到右的填表, 并且每次填一个位置都要遍历dp[0]~dp[i-1]
	// 所以时间复杂度是O(N^2)
	static int subsequenceProblemDP(const vector<int> &vec) {
		vector<int> dp(vec.size(), 1);

		int max = std::numeric_limits<int>::min();
		for (int i = 1; i < vec.size(); i++) {
			for (int j = i - 1; j >= 0; j--)
				dp[i] = vec[i] > vec[j] && dp[j] + 1 > dp[i] ? dp[j] + 1 : dp[i];
			max = std::max(max, dp[i]);
		}

		return *std::max_element(dp.begin(), dp.end());
	}

	// 针对于这种具有枚举行为的动态规划, 可以运用单调性进行加速
	// 设计一个ends数组, ends数组分为有效区和无效区
	// 对于有效区内的值, ends[i]表示所有长度为i+1的递增子序列中最小的结尾
	// 例如: arr=[3, 2, 4, 5, 1, 7], 则ends=[1, 4, 5, 7, x, x]
	// ends[1]表示所有长度为2的递增子序列中最小的结尾为4
	// 所有长度为2的递增子序列有: 24, 25, 27, 45, 47, 57, 17
	// 这些子序列中最小的结尾是4, 所以ends[1]=4
	// 对于ends数组而言, 一定是一个非降序的, 例如所有长度为3的子序列中最小的结尾z(假设是xyz)一定比长度为5的子序列中最小的结尾e(假设是abcde)要小
	// 这是因为ends数组中保存的是递增子序列, 长度长的子序列的结尾一定比长度短的子序列的结尾要大
	// 因此由于ends数组的设计使得上面动态规划原先使用枚举寻找就可以优化为二分查找, 复杂度降低为O(logN)
	// = 诚然, ends数组这样的额外的数据结构的设计天马行空, 不同的题不同, 但是这种运用单调性来加速动态规划的思想却是非常通用且重要的很重要
	static int subsequenceProblemSpeedUp(const vector<int> &vec) {
		vector<int> dp(vec.size(), 1);
		vector<int> ends(vec.size(), 0);

		int validPos = -1;
		for (int i = 0; i < vec.size(); i++) {
			int pos = findFirstGreaterOrEqual(ends, 0, validPos, vec[i]);
			if (pos == -1) {
				ends[++validPos] = vec[i];
				dp[i] = 1;
			} else {
				dp[i] = ends[pos] + 1;
				ends[pos] = ends[pos] > vec[i] ? vec[i] : ends[pos];
			}
		}
		return validPos + 1;
	}

	static int findFirstGreaterOrEqual(const vector<int> &ends, int left, int right, int target) {
		int low = left, high = right;

		if (low > high)
			return -1;

		while (low < high) {
			int mid = left + ((high - low) >> 1);
			if (ends[mid] < target)
				low = mid + 1;
			else
				high = mid;
		}

		if (ends[low] < target)
			return -1;
		return low;
	}
};

class ThreeTimes {
	// 小Q得到一个神奇的数列:1,12,123,...12345678910,1234567891011...。
	// 并且小Q对于能否被3整除这个性质很感兴趣。
	// 小Q现在希望你能帮他计算一下从数列的第l个到第r个(包含端点)有多少个数可以被3整除。
	// 输入描述：
	// 输入包括两个整数l和r(1<=l<=r<=1e9),表示要求解的区间两端。
	// 输出描述：
	// 输出一个整数,表示区间内能被3整除的数字个数。
	// 示例1: 输入2 5, 输出3

public:
	// 思路: 针对每一个数打表, 得到规律, 三个一组false, true, true
	// 所以最终给出left到right区间内的解
	static int threeTimes1(int left, int right) {
		int sum = 0;
		while (left <= right) {
			if ((left - 1) % 3 != 0)
				sum++;
			left++;
		}
		return sum;
	}

	static int threeTimes2(int left, int right) {
		int rightSum = summation(right);
		int leftSum = summation(left);
		return rightSum - leftSum + ((left - 1) % 3 != 0);
	}

	static int summation(int i) {
		return 2 * int((i - 1) / 3) + (i - 1) % 3;
	}

	static void threeTimesTest() {
		string ans = "";
		for (int i = 1; i < 10; i++) {
			ans += std::to_string(i);
			long long nums = std::stoll(ans);
			int sum = 0;
			while (nums > 0)
				sum += nums % 10, nums /= 10;
			cout << i << ": " << std::boolalpha << (sum % 3 == 0) << "\n";
		}
	}
};

class PrintNoInArray {
	// 给定一个整数数组A，长度为n，有1<=A[i]<=n，且对于[1,n]的整数，其中部分整数会重复出现而部分不会出现。
	// 实现算法找到[1,n]中所有未出现在A中的整数。
	// 提示：尝试实现O(n)的时间复杂度和O(1)的空间复杂度（返回值不计入空间复杂度）。
	// 输入描述：
	// 一行数字，全部为整数，空格分隔, 例如: A0 A1 A2 A3...
	// 输出描述：
	// 一行数字，全部为整数，空格分隔, 例如: R0 R1 R2 R3...
	// 示例1:
	// 输入1 3 4 3, 输出2

public:
	// 使用哈希表确实很简单, 难度在于如何使用O(1)的空间复杂度
	// 这里的思路就是因为没有说原数组不能改变, 所以通过交换使得vec中i上的值是i+1
	static void printNoInArray(vector<int> &&vec) {
		for (int i: vec)
			modify(vec, i);

		for (int i = 0; i < vec.size(); i++) {
			if (vec[i] != i + 1)
				cout << i + 1 << " ";
		}
	}

	static void modify(vector<int> &vec, int value) {
		while (vec[value - 1] != value) {
			int tmp = vec[value - 1];
			vec[value - 1] = value;
			value = tmp;
		}
	}
};

class Kiki {
	// CC里面有一个土豪很喜欢一位女直播Kiki唱歌，平时就经常给她点赞、送礼、私聊。
	// 最近CC直播平台在举行中秋之星主播唱歌比赛，假设一开始该女主播的初始人气值为start，能够晋升下一轮人气需要刚好达到end，土豪给主播增加人气的可以采取的方法有：
	//     a.点赞花费xC币，人气+2
	//     b.送礼花费yC币，人气*2
	//     c.私聊花费zC币，人气-2
	// 其中end远大于start且end为偶数，请写一个程序帮助土豪计算一下，最少花费多少C币就能帮助该主播Kiki将人气刚好达到end，从而能够晋级下一轮？
	// 输入描述：
	// 第一行输入5个数据，分别为：x y z start end，每项数据以空格分开。
	// 并且start和end保证为偶数
	// 其中：0＜x,y,z＜＝10000，0＜start, end＜＝1000000
	// 输出描述：
	// 需要花费的最少C币。
	// 示例1:
	// 输入 3 100 1 2 6输出6

public:
	// = 这个问题递归求解其实很简单, 每一步有三种选择, 然后返回三种选择中结果最小的那一个即可
	// = 这个题的意义不在于递归, 而在于很多时候我们可以通过手动求出平凡解, 从而给出题目中没有明说的边界条件, 从而加速递归求解的过程
	static int kiki(int x, int y, int z, int start, int end) {
		// return process1(x, y, z, end, start, 0);
		// 平凡解作为参数传入
		return process2(x, y, z, end, (end - start) / 2 * x, start, 0);
	}

	// 这里递归的难点在于边界条件的确定
	// 这里的递归由于存在减人气的可能, 例如现在的人气是6, 要达到人气是10, x=10万, y=10, z=100, 那就先送礼然后再私聊
	// 因此直接按照下面的这样的写法写的话, 递归实际上是无法终止的
	static int process1(int x, int y, int z, int end, int curr, int used) {
		if (curr == end)
			return 0;
		int dianZan = process1(x, y, z, end, curr + 2, used + x);
		int songLi = process1(x, y, z, end, curr * 2, used + y);
		int siLiao = process1(x, y, z, end, curr - 2, used + z);
		return std::min(dianZan, std::min(songLi, siLiao));
	}

	// 假设现在start=20, end=40, 那么我们实际上有一个平凡解: 连续10次点赞
	// 而我们要找的是最优解, 因此花费一定比这个平凡解要少, 因此在递归的过程中就可以首先手动找到一个平凡解作为一个参数传入作为递归函数的参数, 而后在每次递归的时候判断花费是否超过这个平凡解, 从而实现剪枝
	static int process2(int x, int y, int z, int end, int maxUsed, int curr, int used) {
		if (used > maxUsed)
			return std::numeric_limits<int>::max();
		if (curr == end)
			return used;
		int dianZan = process2(x, y, z, end, maxUsed, curr + 2, used + x);
		int songLi = process2(x, y, z, end, maxUsed, curr * 2, used + y);
		int siLiao = process2(x, y, z, end, maxUsed, curr - 2, used + z);
		return std::min(dianZan, std::min(songLi, siLiao));
	}
};


class MaxRevenue {
	// CC直播的运营部门组织了很多运营活动，每个活动需要花费一定的时间参与，主播每参加完一个活动即可得到一定的奖励，参与活动可以从任意活动开始
	// 但一旦开始，就需要将后续活动参加完毕（注意：最后一个活动必须参与），活动之间存在一定的依赖关系（不存在环的情况）
	// 现在给出所有的活动时间与依赖关系，以及给出有限的时间，请帮主播计算在有限的时候内，能获得的最大奖励，以及需要的最少时长
public:
	struct Activity {
		int day;
		int income;
		vector<pair<int, int>> info;
		vector<Activity *> prev;
	};

	// 本质上就是图的遍历问题, 只需要宽度或者深度优先遍历一次即可
	static pair<int, int> maxRevenue(int maxDay, const vector<vector<int>> &vec) {
		vector<Activity> activities;
		activities.resize(vec.size());

		for (int i = 0; i < vec.size(); i++) {
			activities[i].day = vec[i][0];
			activities[i].income = vec[i][1];
			for (int j = 2; j < vec[i].size(); j++)
				if (vec[i][j] == 1)
					activities[j - 2].prev.push_back(&activities[i]);
		}

		process(activities);

		// 因为可以随意开始, 所以需要整合所有节点中的信息表来获得
		map<int, int> max;
		for (int i = 0; i < activities.size(); i++) {
			for (int j = 0; j < activities[i].info.size(); j++) {
				if (activities[i].info[j].first < maxDay) {
					int day = activities[i].info[j].first;
					int income = activities[i].info[j].second;
					if (!max.contains(day))
						max.insert({day, income});
					else {
						max[day] = std::max(max[day], income);
					}
				}
			}
		}

		map<int, int>::iterator maxIter = max.begin();
		for (auto iter = max.begin(); iter != max.end(); iter++) {
			if (maxIter->second < iter->second)
				maxIter = iter;
		}
		return *maxIter;
	}

	static void process(vector<Activity> &activities) {
		set<Activity *> queuedSet;
		queue<Activity *> que;
		// 最后一个节点
		que.push(&activities[activities.size() - 1]);
		activities.back().info.push_back({activities.back().day, activities.back().income});

		// 宽度优先遍历
		while (!que.empty()) {
			Activity *curr = que.front();
			queuedSet.insert(curr);
			que.pop();
			// 自己的info表中的每一项添加到前向节点中info表中
			for (int i = 0; i < curr->info.size(); i++) {
				for (int j = 0; j < curr->prev.size(); j++) {
					curr->prev[j]->info.push_back({curr->info[i].first + curr->prev[j]->day, curr->info[i].second + curr->prev[j]->income});
				}
			}
			// 添加节点到队列
			for (int i = 0; i < curr->prev.size(); i++)
				if (!queuedSet.contains(curr->prev[i]))
					que.push(curr->prev[i]);
		}
	}
};


int main(int argc, char *argv[]) {
	// 中文数字表达
	cout << "Chinese Expression: " << ChineseExpression::chineseExpression(12001) << "\n";

	// 完全二叉树节点个数
	cout << "Complete Tree Node Number: " << CompleteTreeNodeNumber::completeTreeNodeNumber({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26}) << "\n";

	// 最长递增子序列
	cout << "Envelopes Problem DP: " << SubsequenceProblem::subsequenceProblemDP({3, 2, 4, 5, 1, 7}) << "\n";
	cout << "Envelopes Problem Speed Up: " << SubsequenceProblem::subsequenceProblemSpeedUp({3, 2, 4, 5, 1, 7}) << "\n";

	// 神奇数列
	ThreeTimes::threeTimesTest();
	cout << "ThreeTimes1: " << ThreeTimes::threeTimes1(2, 5) << "\n";
	cout << "ThreeTimes2: " << ThreeTimes::threeTimes2(1, 9) << "\n";

	// 数组中未出现的元素
	cout << "PrintNoInArray: ";
	PrintNoInArray::printNoInArray({1, 3, 4, 3});
	cout << "\n";

	// Kiki
	cout << "Kiki: " << Kiki::kiki(3, 100, 1, 2, 6) << "\n";

	// CC直播
	cout << "Max Revenue: " << MaxRevenue::maxRevenue(10, {{3, 2000, 0, 1, 1, 0, 0, 0, 0, 0}, {3, 4000, 0, 0, 0, 1, 1, 0, 0, 0}, {2, 2500, 0, 0, 0, 1, 0, 0, 0, 0}, {1, 1600, 0, 0, 0, 0, 1, 1, 1, 0}, {4, 3800, 0, 0, 0, 0, 0, 0, 0, 1}, {2, 2600, 0, 0, 0, 0, 0, 0, 0, 1}, {4, 4000, 0, 0, 0, 0, 0, 0, 0, 1}, {3, 3500, 0, 0, 0, 0, 0, 0, 0, 0}})
	     << "\n";
	return 0;
}