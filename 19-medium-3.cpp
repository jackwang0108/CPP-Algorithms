#include <functional>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

using std::cout;
using std::pair;
using std::priority_queue;
using std::string;
using std::unordered_map;
using std::vector;

template<typename T>
std::ostream &operator<<(std::ostream &os, const vector<T> &vec) {
	os << "{";
	for (int i = 0; i < vec.size(); i++)
		os << vec[i] << (i == vec.size() - 1 ? "" : ", ");
	os << "}";
	return os;
}


class SuperWashingMachine {
	// 假设有 n 台超级洗衣机放在同一排上。开始的时候，每台洗衣机内可能有一定量的衣服，也可能是空的。
	// 在每一步操作中，你可以选择任意 m (1 <= m <= n) 台洗衣机，与此同时将每台洗衣机的一件衣服送到相邻的一台洗衣机。
	// 给定一个整数数组 machines 代表从左至右每台洗衣机中的衣物数量，请给出能让所有洗衣机中剩下的衣物的数量相等的 最少的操作步数 。如果不能使每台洗衣机中衣物的数量相等，则返回 -1 。
	// 示例:
	// 输入：machines = [1,0,5]
	// 输出：3
	// 解释：
	// 第一步:    1     0 <-- 5    =>    1     1     4
	// 第二步:    1 <-- 1 <-- 4    =>    2     1     3
	// 第三步:    2     1 <-- 3    =>    2     2     2
public:
	// 这是一个针对业务的题, 针对业务的题做法对描述进行分析, 然后得到具体的业务逻辑
	// 首先是如果衣服总数没有办法被机器数量整除, 那么无论如何都没有办法使得机器上的衣服数量相等, 因此直接返回-1
	// 然后就是针对每个机器进行分析, 每个机器可能的的情况有四个
	// 左侧需要衣服, 右侧需要衣服, 此时轮数至少为左+右
	// 左侧需要衣服, 右侧需要丢出衣服, 此时轮数至少为max(左,右)
	// 左侧需要丢出衣服, 右侧需要衣服, 此时轮数至少为max(左,右)
	// 左侧需要丢出衣服, 右侧需要丢出衣服, 此时轮数至少为max(左,右)
	// 最终的答案就是机器中最痛的那一个, 因为最痛的解决了, 那么剩下的都解决了
	// 这个其实就是一个贪心策略, 贪心策略具体怎么贪心的, 很多时候是需要我们去大量的做题积累的
	// = 而这个洗衣机的题目其实就代表了一类题目, 整个问题的瓶颈就是需要轮数最多的那个机器, 只要这个机器能够解决, 那么所有的机器就一定都解决了
	static int superWashingMachine(const vector<int> &vec) {
		int sum = 0;
		for (int i: vec)
			sum += i;
		if (sum % vec.size() != 0)
			return -1;

		int result = 0;
		int leftClothSum = 0;
		int average = sum / vec.size();
		for (int i = 0; i < vec.size(); i++) {
			int leftNeedCloth = leftClothSum - i * average;
			int rightNeedCloth = (sum - leftClothSum - vec[i]) - (vec.size() - i - 1) * average;
			if (leftNeedCloth < 0 && rightNeedCloth < 0)
				result = std::max(result, std::abs(leftNeedCloth) + std::abs(rightNeedCloth));
			else
				result = std::max(result, std::max(std::abs(leftNeedCloth), std::abs(rightNeedCloth)));
			leftClothSum += vec[i];
		}
		return result;
	}
};

class PrintMatrixSpiralOrder {
	// 用螺旋的方式打印矩阵，比如如下的矩阵:
	// 0 1 2 3
	// 4 5 6 7
	// 8 9 10 11
	// 打印顺序为：0 1 2 3 7 11 10 9 8 4 5 6
public:
	// 这种题就是不要考虑怎么移动, 而是宏观的调度, 使用一个函数打印一个框即可
	static void printFrame(const vector<vector<int>> &vec, int upperLeftX, int upperLeftY, int bottomRightX, int bottomRightY) {
		if (upperLeftX == bottomRightX) {
			for (int i = upperLeftY; i <= bottomRightY; i++)
				cout << vec[i][upperLeftX] << " ";
		} else if (upperLeftY == bottomRightY) {
			for (int i = upperLeftX; i <= bottomRightX; i++)
				cout << vec[upperLeftY][i] << " ";
		} else {
			for (int i = upperLeftX; i < bottomRightX; i++)
				cout << vec[upperLeftY][i] << " ";
			for (int i = upperLeftY; i < bottomRightY; i++)
				cout << vec[i][bottomRightX] << " ";
			for (int i = bottomRightX; i > upperLeftX; i--)
				cout << vec[bottomRightY][i] << " ";
			for (int i = bottomRightY; i > upperLeftY; i--)
				cout << vec[i][upperLeftX] << " ";
		}
	}

	static void printMatrixSpiralOrder(const vector<vector<int>> &vec) {
		int upperLeftX = 0, upperLeftY = 0;
		int bottomRightX = vec[0].size() - 1, bottomRightY = vec.size() - 1;
		while (upperLeftX <= bottomRightX && upperLeftY <= bottomRightY)
			printFrame(vec, upperLeftX++, upperLeftY++, bottomRightX--, bottomRightY--);
	}
};


class RotateMatrix {
	// 给定一个正方形矩阵，只用有限几个变量
	// 实现矩阵中每个位置的数顺时针转动90度，比如如下的矩阵:
	// 0   1  2  3
	// 4   5  6  7
	// 8   9 10 11
	// 12 13 14 15
	// 矩阵应该被调整为：
	// 12  8  4  0
	// 13  9  5  1
	// 14  10 6  2
	// 15  11 7  3
public:
	// 和上面的题一样的思路
	static vector<vector<int>> rotateMatrix(const vector<vector<int>> &vec) {
		int a = 0, b = 0, c = vec.size() - 1, d = vec.size() - 1;
		vector<vector<int>> copy = vec;
		while (a < c)
			rotateEdge(copy, a++, b++, c--, d--);
		return copy;
	}

	static void rotateEdge(vector<vector<int>> &vec, int a, int b, int c, int d) {
		int temp = 0;
		for (int i = 0; i < d - b; i++) {
			temp = vec[a][b + i];
			vec[a][b + i] = vec[c - i][b];
			vec[c - i][b] = vec[c][d - i];
			vec[c][d - i] = vec[a + i][d];
			vec[a + i][d] = temp;
		}
	}
};

class ZigzagPrint {
	// 用zigzag的方式打印矩阵，比如如下的矩阵:
	// 0 1 2 3
	// 4 5 6 7
	// 8 9 10 11
	// 打印顺序为：0 1 4 8 5 2 3 6 9 10 7 11
public:
	// 和上面的思路类似, 不要纠结于某一个位置的移动, 考虑一个整体, 这里的整体就是斜线
	static void zigzagPrint(const vector<vector<int>> &nums) {
		int topRow = 0, topCol = 0;
		int downRow = 0, downCol = 0;
		int endRow = nums.size() - 1, endCol = nums[0].size() - 1;
		bool topToDown = false;
		while (topRow != endRow + 1) {
			printDiagonal(nums, topRow, topCol, downRow, downCol, topToDown);
			topRow = topCol == endCol ? topRow + 1 : topRow;
			topCol = topCol == endCol ? topCol : topCol + 1;
			downCol = downRow == endRow ? downCol + 1 : downCol;
			downRow = downRow == endRow ? downRow : downRow + 1;
			topToDown = !topToDown;
		}
	}

	static void printDiagonal(const vector<vector<int>> &nums, int topRow, int topCol, int downRow, int downCol, bool topToDown) {
		if (topToDown)
			while (topRow != downRow + 1)
				cout << nums[topRow++][topCol--] << " ";
		else
			while (downRow != topRow - 1)
				cout << nums[downRow--][downCol++] << " ";
	}
};

class SplitNbySM {
	// 假设s和m初始化，s="a", m=s
	// 再定义两种操作，
	//   第一种操作：m=s, s=s+s
	//   第二种操作：s=s+m
	// 求最小的操作步骤数，可以将s拼接到长度等于n
public:
	static int splitNbySM(int n) {
		return processSplitNbyM("a", "a", n);
	}

	static int processSplitNbyM(string s, string m, int n) {
		if (s.length() == n)
			return 0;
		if (s.length() > n)
			return -1;
		int firstOp = processSplitNbyM(s + s, s, n);
		int secondOp = processSplitNbyM(s + m, m, n);
		if (firstOp == -1 && secondOp == -1)
			return -1;
		else {
			if (firstOp == -1)
				return secondOp + 1;
			if (secondOp == -1)
				return firstOp + 1;
			return std::min(firstOp, secondOp) + 1;
		}
	}

	// 暴力打表, 这个题也可以暴力打表, 但是很难观察出规律
	// 所以这里直接给出数学原理
	// 规则一表示s倍增, s.len=2*s.len, 规则二表示s增加指定长度, s.len=s.len+m.len
	// 所以如果现在n是一个质数, 那么n.len=1*n.len,所以一定不会用到规则一, 因为如果中间用到规则一的话, 假设一共进行n次操作, 前k次是操作二, 第k+1次是操作一, 此后都是操作二, 则最终的长度的表达式是n=2*(k) + k*(n-k-1)=k*(n-k+1), 最后长度n就不是质数, 因此如果长度是质数的话, 就一定只会进行第二种操作
	// 如果现在n是一个合数, 那么就把合数拆分成质数相乘的形式. 此时每个质数表示操作二的个数, 两个指数相乘表示操作一的个数
	static int splitNbySMMath(int n) {
		if (n < 2)
			return 0;
		if (isPrime(n))
			return n - 1;
		auto [sum, count] = divSumAndCount(n);
		return sum - count;
	}

	static bool isPrime(int num) {
		if (num <= 1) {
			return false;// 1和负数都不是质数
		}
		for (int i = 2; i * i <= num; i++) {
			if (num % i == 0) {
				return false;// 如果能整除，说明不是质数
			}
		}
		return true;// 如果都不能整除，说明是质数
	}


	// 返回非质数n的质分解的和, 不包括以
	static pair<int, int> divSumAndCount(int n) {
		int sum = 0;
		int count = 0;
		for (int i = 2; i <= n; i++) {
			while (n % i == 0) {
				sum += i;
				count++;
				n /= i;
			}
		}
		return {sum, count};
	}
};


class TopKTimes {
	// 给定一个字符串类型的数组arr，求其中出现次数最多的前K个字符串
public:
	static vector<string> topKTimes(const vector<string> &vec, int k) {
		unordered_map<string, int> freq;
		for (const string &str: vec) {
			if (freq.contains(str))
				freq[str] += 1;
			else
				freq[str] = 1;
		}

		std::function<bool(pair<string, int> &, pair<string, int> &)> func = [](pair<string, int> &p1, pair<string, int> &p2) -> bool {
			return p1.second < p2.second;
		};
		priority_queue<pair<string, int>, vector<pair<string, int>>, decltype(func)> que(func);
		for (auto i = freq.begin(); i != freq.end(); i++)
			que.push(*i);

		vector<string> result;
		for (int i = 0; i < k; i++)
			result.push_back(que.top().first), que.pop();
		return std::move(result);
	}
};


int main(int argc, char *argv[]) {
	// 洗衣机问题
	cout << "Super Washing Machine: " << SuperWashingMachine::superWashingMachine(vector<int>{1, 0, 5}) << "\n";

	// 螺旋打印矩阵
	cout << "Print Matrix Spiral Order: ";
	PrintMatrixSpiralOrder::printMatrixSpiralOrder(vector<vector<int>>{
	    {0, 1, 2, 3},
	    {4, 5, 6, 7},
	    {8, 9, 10, 11}});
	cout << "\n";

	// 旋转矩阵
	cout << "Rotate Matrix: " << RotateMatrix::rotateMatrix(vector<vector<int>>{{0, 1, 2, 3}, {4, 5, 6, 7}, {8, 9, 10, 11}, {12, 13, 14, 15}}) << "\n";

	// Zigzag打印
	cout << "Zigzag Print: ";
	ZigzagPrint::zigzagPrint(vector<vector<int>>{
	    {0, 1, 2, 3},
	    {4, 5, 6, 7},
	    {8, 9, 10, 11}});
	cout << "\n";

	// 最小操作数
	cout << "split Nby SM: " << SplitNbySM::splitNbySM(20) << "\n";
	cout << "split Nby SM Math: " << SplitNbySM::splitNbySMMath(20) << "\n";

	// top k问题
	cout << "Top K Times: " << TopKTimes::topKTimes({"abc", "abc", "bck", "efg", "efg", "efg", "bck", "bck"}, 2) << "\n";
	return 0;
}