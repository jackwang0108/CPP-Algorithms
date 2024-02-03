#include <functional>
#include <iostream>
#include <queue>
#include <stack>
#include <string>
#include <utility>
#include <vector>

using std::cout;
using std::pair;
using std::priority_queue;
using std::stack;
using std::string;
using std::vector;

class GetMinStack {
	// 实现一个特殊的栈，在实现栈的基本功能的基础上，再实现返回栈中最小元素的操作。
	// 要求：
	//  1.pop、push、getMin操作的时间复杂度都是O(1)；
	//  2.设计的栈类型可以使用现成的栈结构

public:
	class MinStack {
	private:
		stack<int> st;
		stack<int> minSt;

	public:
		MinStack() {}
		void push(int value) {

			minSt.push((st.empty() || value < st.top()) ? value : st.top());
			st.push(value);
		}

		int pop() {
			int value = st.top();
			st.pop();
			minSt.pop();
			return value;
		}

		int getMin() {
			return minSt.top();
		}
	};

	static void getMinStack(const vector<int> &vec) {
		MinStack m;
		for (auto n: vec) {
			m.push(n);
			cout << "Current Min: " << m.getMin() << "\n";
		}
	}
};


class MinPathSum {
	// 动态规划的空间压缩技巧
	// 给你一个二维数组matrix，其中每个数都是正数，要求从左上角走到右下角。
	// 每一步只能向右或者向下，沿途经过的数字要累加起来。最后请返回最小的路径和。
public:
	static int minPathSumRecursion(const vector<vector<int>> &vec) {
		return processMinPathSum(vec, 0, 0);
	}

	static int processMinPathSum(const vector<vector<int>> &vec, int row, int col) {
		if (row >= vec.size() || col >= vec[row].size())
			return -1;
		if (row == vec.size() - 1 && col == vec[vec.size() - 1].size() - 1)
			return vec[vec.size() - 1][vec[vec.size() - 1].size() - 1];
		int downSum = processMinPathSum(vec, row + 1, col);
		int rightSum = processMinPathSum(vec, row, col + 1);
		if (downSum == -1 && rightSum == -1)
			return -1;
		else {
			if (downSum == -1)
				return rightSum + vec[row][col];
			if (rightSum == -1)
				return downSum + vec[row][col];
			return std::min(rightSum, downSum) + vec[row][col];
		}
	}

	// 返回f(0,0), 每次填表依赖下方和右侧的值, 所以填表是从下到上, 从右到左
	// basecase就是越界是-1, 终点的值就是它的值
	static int minPathSumDP(const vector<vector<int>> &vec) {
		vector<vector<int>> dp;
		dp.resize(vec.size() + 1);
		for (int i = 0; i <= vec.size(); i++)
			dp[i].resize(vec[i == vec.size() ? i - 1 : i].size() + 1);

		for (int i = 0; i < vec.size(); i++)
			dp[i][vec[i].size()] = -1;
		for (int i = 0; i <= dp[vec.size()].size(); i++)
			dp[vec.size()][i] = -1;
		dp[vec.size() - 1][vec[vec.size() - 1].size() - 1] = vec[vec.size() - 1][vec[vec.size() - 1].size() - 1];

		for (int rowIdx = vec.size() - 1; rowIdx >= 0; rowIdx--) {
			for (int colIdx = vec[rowIdx].size() - 1; colIdx >= 0; colIdx--) {
				if (dp[rowIdx + 1][colIdx] == -1 && dp[rowIdx][colIdx + 1] == -1)
					continue;
				if (dp[rowIdx + 1][colIdx] == -1)
					dp[rowIdx][colIdx] = vec[rowIdx][colIdx] + dp[rowIdx][colIdx + 1];
				else if (dp[rowIdx][colIdx + 1] == -1)
					dp[rowIdx][colIdx] = vec[rowIdx][colIdx] + dp[rowIdx + 1][colIdx];
				else {
					dp[rowIdx][colIdx] = vec[rowIdx][colIdx] + std::min(dp[rowIdx + 1][colIdx], dp[rowIdx][colIdx + 1]);
				}
			}
		}
		return dp[0][0];
	}

	// 动态规划问题我们很多时候要的都是表的左上角或者右上角, 并且填表按照从上到下从左到右或者从下到上从左到右的依赖
	// 因此动态规划在填表的时候其实没必要申请整张表, 可以直接申请一行即可
	static int minPathSumDPBetter(const vector<vector<int>> &vec) {
		// 实际上还能再压缩, 因为thisRow每次只用到了下方的值, 所以没必要再开一个lastRow
		vector<int> lastRow(vec[0].size() + 1, -1);
		vector<int> thisRow(vec[0].size() + 1, 0);

		thisRow[vec[0].size()] = -1;
		for (int rowIdx = vec.size() - 1; rowIdx >= 0; rowIdx--) {
			for (int colIdx = vec[rowIdx].size() - 1; colIdx >= 0; colIdx--) {
				if (lastRow[colIdx] == -1 && thisRow[colIdx + 1] == -1)
					thisRow[colIdx] = vec[rowIdx][colIdx];
				else if (lastRow[colIdx] == -1)
					thisRow[colIdx] = vec[rowIdx][colIdx] + thisRow[colIdx + 1];
				else if (thisRow[colIdx + 1] == -1)
					thisRow[colIdx] = vec[rowIdx][colIdx] + lastRow[colIdx];
				else
					thisRow[colIdx] = vec[rowIdx][colIdx] + std::min(lastRow[colIdx], thisRow[colIdx + 1]);
			}
			std::swap(lastRow, thisRow);
		}
		return lastRow[0];
	}
};

class WaterProblem {
	// 给定一个数组arr，已知其中所有的值都是非负的，将这个数组看作一个容器，请返回容器能装多少水
	// 比如，arr={3，1，2，5，2，4}，根据值画出的直方图就是容器形状，该容器可以装下5格水
	// 再比如，arr={4，5，1，3，2}，该容器可以装下2格水
public:
	// 这个题如果从找局部最小值的角度出发, 那么时间复杂度就爆炸了, 因为局部最小值可能是另外一个的局部最小值
	// 例如下面的这个情况, 找到了三个波谷, 但是整体都在5的这个波谷里面, 所以其实这三次波谷找的都是没意义的
	/* 
     * 5           5
     * 5           5
     * 5   3   3   5
     * 5   3   3   5
     * 5 1 3 1 3 1 5
     */
	// 所以更好的策略是每个点光看自己头上能装多少水, 决定自己头上能装多少水就是看左侧的点和右侧的点能装多少水
	// 因为左侧的点能装多少水就以为当前点遥远的左侧由一个边界>=左侧点能装的水
	// 右侧同理
	// 所以当前点能装的水就是max(min(左侧点能装的水, 右侧点能装的水) - arr[i], 0)
	// 和0取max是考虑到当前点很高, 装不下水, 所以此时就是0
public:
	static int waterProblem(const vector<int> &arr) {
		int all = 0;
		int leftMax = arr[0], rightMax = arr[arr.size() - 1];
		int leftPos = 1, rightPos = arr.size() - 2;
		while (leftPos < rightPos) {
			if (leftMax < rightMax)
				all += std::max(leftMax - arr[leftPos++], 0);
			else
				all += std::max(rightMax - arr[rightPos--], 0);
			leftMax = std::max(leftMax, arr[leftPos]);
			rightMax = std::max(rightMax, arr[rightMax]);
		}
		return all;
	}
};


class MaxABSBetweenLeftAndRight {
	// 给定一个数组arr长度为N，你可以把任意长度大于0且小于N的前缀作为左部分，剩下的作为右部分。但是每种划分下都有左部分的最大值和右部分的最大值
	// 请返回最大的，左部分最大值减去右部分最大值的绝对值。
public:
	// 这个题的思路就是一侧的max尽量大, 另外一个的max尽量小
	// 而数组会有一个max, 所以尽量大的一侧的max就是全局最大
	// 那问题就是求另外一侧的max什么时候最小?
	// 答案就是边界. 如果max在左侧, 那么右侧max至少会有最后一个数, 则此时右侧max最小就是这个数
	// 因为如果右侧再加一个数, 如果新的数比这个数大, 那么max就会变大, 如果新的数比这个数小, 那么max不变
	// 左侧同理
	// 所以总结下来流程很简单, 就是max - 左右两侧较小的值
	static int maxABSBetweenLeftAndRight(vector<int> vec) {
		int max = vec[0];
		for (auto n: vec)
			max = std::max(max, n);
		return std::abs(max - std::min(vec[0], vec[vec.size() - 1]));
	}
};

class IsRotation {
	// 如果一个字符串为str，把字符串str前面任意的部分挪到后面形成的字符串叫作str的旋转词。
	// 比如str="12345"，str的旋转词有"12345"、"23451"、"34512"、"45123"和"51234"。
	// 给定两个字符串a和b，请判断a和b是否互为旋转词。
	// 比如：
	//  a="cdab"，b="abcd"，返回true。
	//  a="1ab2"，b="ab12"，返回false。
	//  a="2ab1"，b="ab12"，返回true。
public:
	// 思路很简单, 就是看b是否是a+a的子字符串
	static bool isRotation(const string &a, const string &b) {
		return a.length() == b.length() && ((a + a).find(b) != string::npos);
	}
};


class DrinkCoffee {
	// 给你几个数据：
	// 数组arr：表示几个咖啡机，这几个咖啡机生产一杯咖啡所需要的时间就是数组中的值, 例如arr=[2,3,7]就表示第一台咖啡机生产一杯咖啡需要2单位时间，第二台需要3单位时间，第三台需要7单位时间。
	// int N：表示有N个人需要用咖啡机制作咖啡，每人一杯，同时，假设制作完咖啡后，喝咖啡时间为0，一口闷。
	// int a：表示用洗碗机洗一个咖啡杯需要的时间，串行运行。
	// int b：表示咖啡杯也可以不洗，自然晾干的时间，咖啡杯要么洗，要么晾干。
	// 现在，请你求出这N个人从开始用咖啡杯制作咖啡到杯子洗好或者晾干的最少时间？
public:
	// 业务题
	// 分析问题的关键就是在于时间的拆分
	// 整个过程中需要的时间其实就是两个:
	//  1. 所有人从开始排队到喝完咖啡所需要的时间
	//  2. 从开始洗第一个杯子到洗完最后一个杯子的时间
	static int drinkCoffee(const vector<int> &vec, int N, int a, int b) {
		std::function<bool(pair<int, int> &, pair<int, int> &)> func = [](pair<int, int> &p1, pair<int, int> &p2) -> bool {
			return p1.first + p1.second > p2.first + p2.second;
		};

		priority_queue<pair<int, int>, vector<pair<int, int>>, decltype(func)> freeMachineQueue(func);
		for (auto &i: vec)
			freeMachineQueue.push(std::make_pair(0, i));

		// 模拟喝咖啡, 那个机器能让用户最早喝到咖啡就用哪一台
		vector<int> drinkingTime;
		for (int i = 0; i <= N; i++) {
			pair<int, int> machine = freeMachineQueue.top();
			freeMachineQueue.pop();
			drinkingTime.push_back(machine.first + machine.second);
			machine.first += machine.second;
			freeMachineQueue.push(machine);
		}

		// 模拟洗杯子
		return washAllCups(drinkingTime, a, b, 0, 0);
	}

	// 这里模拟洗杯子时候需要注意:
	//  1. basecase: 只剩一个咖啡杯, 此时返回洗碗机洗杯子和自然晾干洗杯子最早结束的
	//  2. 洗碗机不是随时都可以用的, 所以洗这个杯子时候洗碗机可用的时间需要作为参数传入
	static int washAllCups(vector<int> &startWashingTime, int a, int b, int index, int currWashingMachineAvailableTime) {
		if (index == startWashingTime.size() - 1)
			// 如果要使用洗碗机机的话需要等到下一次可以使用的时间
			return std::min(std::max(currWashingMachineAvailableTime, startWashingTime[index]) + 1, startWashingTime[index] + b);

		// 这一次使用洗碗机洗杯子花费的时间
		int nextWashingMachineAvailableTime = std::max(currWashingMachineAvailableTime, startWashingTime[index]) + a;
		int restTime1 = washAllCups(startWashingTime, a, b, index + 1, nextWashingMachineAvailableTime);
		int p1 = std::max(nextWashingMachineAvailableTime, restTime1);

		int dryThisCup = startWashingTime[index] + b;
		int restTime2 = washAllCups(startWashingTime, a, b, index + 1, currWashingMachineAvailableTime);
		int p2 = std::max(dryThisCup, restTime2);

		return std::min(p1, p2);
	}
};


int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
	// GetMinStack问题
	GetMinStack::getMinStack({3, 2, 4, 1});

	// MinPathSum问题
	cout << "Min Path Sum Recursion: " << MinPathSum::minPathSumRecursion(vector<vector<int>>{{1, 2, 3, 4, 5}, {6, 7, 8, 9, 10}, {11, 12, 13, 14, 15}}) << "\n";
	cout << "Min Path Sum DP: " << MinPathSum::minPathSumDP(vector<vector<int>>{{1, 2, 3, 4, 5}, {6, 7, 8, 9, 10}, {11, 12, 13, 14, 15}}) << "\n";
	cout << "Min Path Sum DP Better: " << MinPathSum::minPathSumDPBetter(vector<vector<int>>{{1, 2, 3, 4, 5}, {6, 7, 8, 9, 10}, {11, 12, 13, 14, 15}}) << "\n";

	// 接雨水问题
	cout << "Water Problem: " << WaterProblem::waterProblem({3, 1, 2, 5, 2, 4}) << "\n";

	// 旋转字符串问题
	cout << std::boolalpha;
	cout << "Is Rotation: " << IsRotation::isRotation("cdab", "abcd") << "\n";
	cout << "Is Rotation: " << IsRotation::isRotation("1ab2", "ab12") << "\n";
	cout << "Is Rotation: " << IsRotation::isRotation("2ab1", "ab12") << "\n";

	// 喝咖啡问题
	cout << "Drink Coffee: " << DrinkCoffee::drinkCoffee(vector<int>{3, 2, 7}, 10, 2, 3) << "\n";
	return 0;
}