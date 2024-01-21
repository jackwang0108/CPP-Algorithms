#include <iostream>
#include <stack>
#include <string>
#include <unordered_set>
#include <vector>

using std::cout;
using std::stack;
using std::string;
using std::unordered_set;
using std::vector;

// 不论是分治, 还是递归, 还是回溯, 本质上都是暴力递归, 其核心就是尝试, 只不过尝试时采用的策略不同
// 如果通过暴力递归得到了问题的解, 那么会有大量的理论去把暴力递归转化为动态规划, 在后面的提升班中就是一步的事
// 因此这节课不拘泥于专业的术语名词, 统一使用暴力递归来描述这些算法
// 而且国外的算法课其实不会讲这些名词, 只会讲force recursive

// 暴力递归就是尝试
// 1，把问题转化为规模缩小了的同类问题的子问题, 当问题的规模小到什么时候不需要再往下试了, 直接就能给出答案的, 就是basecase
// 2，有明确的不需要继续进行递归的条件(basecase)
// 3，有当得到了子问题的结果之后的决策过程
// 4，不记录每一个子问题的解一定要学会怎么去尝试，因为这是动态规划的基础，这一内容我们将在提升班讲述题目一

// 而回溯, 分支限界 其实都是剪枝条件

// = 考虑问题的核心就是如何在第i-1个问题已经解决的基础上解决第i个问题


class Hanoi {
	// 汉诺塔问题: 打印n层汉诺塔从最左边移动到最右边的全部过程
public:
	// 汉诺塔的递归方式就是
	// 现在要把前i个放到右边, 那么第一步就是把前i-1放到中间, 然后把第i个放到右边, 接下来把前i-1个放到右边
	static void hanoi(int n) {
		if (n <= 0)
			return;
		process(n, "左", "右", "中");
	}

	static void process(int i, const string &from, const string &to, const string &other) {// NOLINT(*-no-recursion)
		if (i == 1)
			cout << "Move 1 from " << from << " to " << to << "\n";
		else {
			process(i - 1, from, other, to);
			cout << "Move " << i << " from " << from << " to " << to << "\n";
			process(i - 1, other, to, from);
		}
	}
};

class PrintSubString {
	// 打印一个字符串的全部子序列，包括空字符串
public:
	// 递归方式: 假设前i-1的子字符串已经打印, 那么打印前i的子字符串就是不要第i个字符和要第i个字符两种方式
	// 递归的结束方式就是i位置就是字符串的长度
	static void printSubString(const string &str) {
		if (str.empty())
			return;
		process(str, 0);
	}

	static void process(const string &str, int i) {// NOLINT(*-no-recursion)
		if (i >= str.length()) {
			cout << str << "\n";
			return;
		}
		// 要当前字符
		process(str, i + 1);
		// 不要当前字符
		string copy = str;
		copy.erase(i);
		process(copy, i + 1);
	}
};

class PrintAllPermutation {
	// 打印一个字符串的全部排列，要求不要出现重复的排列
public:
	// 递归方式就是, 假设前i-1的字符都是已经全排列过的, i~N的字符的全排列就是i~N的字符全都在第i位 + i+1位到N的全排列
	// = 递归的时候可以传递一个参数进去保存结果, 利用这个结果, 后面的课程会优化为动态规划
	static void printAllPermutation(const string &str) {
		if (str.empty())
			return;
		string copy = str;
		unordered_set<string> result;
		process1(copy, 0, result);

		for (auto begin = result.begin(); begin != result.end(); begin++)
			cout << *begin << "\n";
	}

	static void process1(string &str, int i, unordered_set<string> &result) {// NOLINT(*-no-recursion)
		if (i == str.size())
			result.insert(str);
		for (int swapIdx = i; swapIdx < str.size(); swapIdx++) {
			std::swap(str[i], str[swapIdx]);
			process1(str, i + 1, result);
			std::swap(str[i], str[swapIdx]);
		}
	}

	// 可能会出现abac这样的字符串, 在全排列的时候需要去掉重复的全排列
	static void printAllPermutationWithoutSame(const string &str) {
		if (str.empty())
			return;
		string copy = str;
		unordered_set<string> result;
		process2(copy, 0, result);

		for (auto begin = result.begin(); begin != result.end(); begin++)
			cout << *begin << "\n";
	}

	// = 去掉重复的全排列其实就是检查这个字符是否已经在第i位上排列过了, 如果排列过了直接过
	// = 这其实就是递归时候的剪枝, 也就是分支限界, 在递归的时候提前杀死不可能的分支, 从而加速递归的过程
	// = 这比提前加到结果里, 然后对结果去重要好
	static void process2(string &str, int i, unordered_set<string> &result) {
		if (i == str.size())
			result.insert(str);
		bool swapped[26]{false};
		for (int swapIdx = i; swapIdx < str.size(); swapIdx++) {
			if (!swapped[str[swapIdx] - 'a']) {
				swapped[str[swapIdx] - 'a'] = true;
				std::swap(str[i], str[swapIdx]);
				process2(str, i + 1, result);
				std::swap(str[i], str[swapIdx]);
			}
		}
	}
};


class CardsInLine {
	// 给定一个整型数组arr，代表数值不同的纸牌排成一条线。
	// 玩家A和玩家B依次拿走每张纸牌，规定玩家A先拿，玩家B后拿，但是每个玩家每次只能拿走最左或最右的纸牌
	// 玩家A和玩家B都绝顶聪明。请返回最后获胜者的分数。
	// 【举例】
	// arr=[1,2,100,4]。开始时，玩家A只能拿走1或4。
	// 如果开始时玩家A拿走1，则排列变为[2,100,4]，接下来玩家B可以拿走2或4，然后继续轮到玩家A...
	// 如果开始时玩家A拿走4，则排列变为[1,2,100]，接下来玩家B可以拿走1或100，然后继续轮到玩家A...
	// 玩家A作为绝顶聪明的人不会先拿4，因为拿4之后，玩家B将拿走100。
	// 所以玩家A会先拿1，让排列变为[2,100,4]，接下来玩家B不管怎么选，100都会被玩家A拿走。
	// 玩家A会获胜，分数为101。所以返回101。arr=[1,100,2]。开始时，玩家A不管拿1还是2，玩家B作为绝顶聪明的人，都会把100拿走。
	// 玩家B会获胜，分数为100。所以返回100。

public:
	// 递归策略就是: 玩家A这次抽左侧牌的结果 + 下次抽牌的结果 和 玩家A抽右侧牌的结果 + 下次抽牌的结果中选择最大的
	// 下次抽牌前需要让玩家B选择让玩家A抽牌结果最不好的结果
	static int cardsInLineWinner(const vector<int> &vec) {
		if (vec.empty())
			return 0;
		// 玩家A是先手拿牌, 玩家B是后手拿牌
		return std::max(firstHand(vec, 0, vec.size() - 1), secondHand(vec, 0, vec.size() - 1));
	}

	// left ~ right 范围内先手拿牌
	static int firstHand(const vector<int> &vec, int left, int right) {
		// 如果left == right，意味着只剩下一张牌，玩家A直接拿走这张牌。
		if (left == right)
			return vec[right];
		// 玩家A拿走左侧的牌 + 自己下一次抽牌的结果(玩家B会让玩家A下一次抽牌的结果最不好)
		// 玩家A拿走右侧的牌 + 自己下一次抽牌的结果(玩家B会让玩家A下一次抽牌的结果最不好)
		// 两个结果中返回最大的
		return std::max(vec[left] + secondHand(vec, left + 1, right), vec[right] + secondHand(vec, left, right - 1));
	}

	static int secondHand(const vector<int> &vec, int left, int right) {
		// 如果left == right，意味着只剩下一张牌，但玩家B没有机会拿牌，因为玩家A会先拿走它，所以返回0。
		if (left == right)
			return 0;
		// 玩家B抽牌的让玩家A下一次抽牌结果最不利的牌
		return std::min(firstHand(vec, left + 1, right), firstHand(vec, left, right - 1));
	}
};

template<typename T>
std::ostream &operator<<(std::ostream &os, const stack<T> &st) {
	stack<T> copy = st;
	os << "Top [";
	while (!copy.empty()) {
		os << copy.top();
		copy.pop();
		if (!copy.empty())
			os << ", ";
	}
	os << "] Bottom";
	return os;
}

class ReverseStack {
	// 给你一个栈，请你逆序这个栈，不能申请额外的数据结构，只能使用递归函数。如何实现?
public:
	// 递归策略: 每次pop出栈底的值, 然后把剩下的栈逆序之后再这个值压到栈顶
	static void reverseStack(stack<int> &st) {
		if (st.empty())
			return;
		int bottom = popBottom(st);
		reverseStack(st);
		st.push(bottom);
	}

	// 递归策略: 弹出一个值后如果栈为空, 则返回这个值, 如果栈不为空, 则处理剩下的栈
	static int popBottom(stack<int> &st) {
		int curr = st.top();
		st.pop();
		if (st.empty())
			return curr;
		else {
			int last = popBottom(st);
			st.push(curr);
			return last;
		}
	}
};

class ConvertString {
	// 规定1和A对应、2和B对应、3和C对应...
	// 那么一个数字字符串比如"111"，就可以转化为"AAA"、"KA"和"AK"。
	// 给定一个只有数字字符组成的字符串str，返回有多少种转化结果。
public:
	// 递归策略: 当前位置选择一位或者两位, 然后递归处理剩下的
	static int convertString(const string &str) {
		if (str.empty())
			return 0;
		return process(str, 0);
	}

	static int process(const string &str, int i) {
		if (i == str.length())
			return 1;
		if (str[i] == '0')
			return 0;
		if (str[i] == '1') {
			int result = process(str, i + 1);
			if (i + 1 < str.length())
				result += process(str, i + 2);
			return result;
		}
		if (str[i] == '2') {
			int result = process(str, i + 1);
			if (i + 1 < str.length() && '0' <= str[i] && str[i] <= '6')
				result += process(str, i + 2);
			return result;
		}
		return process(str, i + 1);
	}
};

class Knapsack {
	// 给定两个长度都为N的数组weights和values
	// weights[i]和values[i]分别代表i号物品的重量和价值
	// 给定一个正数bag，表示一个载重bag的袋子，你装的物品不能超过这个重量。
	// 返回你能装下最多的价值是多少？
public:
	// 递归策略: 如果能装这个物体, 装和不装这个物体以及剩余的最大值返回
	static int knapsack(const vector<int> &weights, const vector<int> &values, int bagSize) {
		if (bagSize == 0 || weights.empty())
			return 0;
		return process(weights, values, 0, bagSize);
	}

	// = 在递归的时候, 固定参数不管, 可变参数函形式越简单(给个int, 别搞个链表), 可变参数越少的做法, 越容易修改为动态规划
	static int process(const vector<int> weights, const vector<int> &values, int idx, int bagSize) {
		if (idx >= weights.size())
			return 0;
		int getValue = 0;
		if (weights[idx] <= bagSize) {
			getValue = values[idx] + process(weights, values, idx + 1, bagSize - weights[idx]);
		}
		int noGetValue = process(weights, values, idx + 1, bagSize);
		return std::max(getValue, noGetValue);
	}
};

class NQueens {
	// N皇后问题是指在N*N的棋盘上要摆N个皇后，要求任何两个皇后不同行、不同列，也不在同一条斜线上。
	// 给定一个整数n，返回n皇后的摆法有多少种。n=1，返回1。n=2或3，2皇后和3皇后问题无论怎么摆都不行，返回0。n=8，返回92。

public:
	// 递归策略: 依次选择之前的确定下来之后合法的位置, 然后调用下一层
	static int nQueens1(int n) {
		if (n < 1)
			return 0;

		vector<int> queenPos(n, 0);
		return process1(0, queenPos, n);
	}

	// process的前提就是0 ~ rowIdx-1行的皇后的位置都是已经排好的, 不会发生冲突(同行, 同列, 同斜线)
	static int process1(int rowIdx, vector<int> &queensPos, int numRows) {// NOLINT(*-no-recursion)
		if (rowIdx == numRows)
			return 1;
		// 依次检查0-numRows列 (棋盘是方形的)
		int result = 0;
		for (int colIdx = 0; colIdx < numRows; colIdx++) {
			// 当前的列能否放该皇后
			if (isValidPos1(queensPos, rowIdx, colIdx)) {
				queensPos[rowIdx] = colIdx;
				result += process1(rowIdx + 1, queensPos, numRows);
			}
		}
		return result;
	}

	static bool isValidPos1(vector<int> &queenPos, int rowIdx, int colIdx) {
		for (int i = 0; i < rowIdx; i++)
			if (queenPos[i] == colIdx || std::abs(colIdx - queenPos[i]) == std::abs(rowIdx - i))
				return false;
		return true;
	}


	// 使用位运算加速的版本, 指标还是(N^N), 但是常数时间可以优化
	static int nQueens2(int n) {
		if (n < 1 || n > 32)
			return 0;

		int limit = n == 32 ? -1 : (1 << n) - 1;
		return process2(limit, 0, 0, 0);
	}

	static int process2(int limit, int colLimit, int leftLimit, int rightLimit) {// NOLINT(*-no-recursion)
		if (limit == colLimit)
			return 1;
		int availablePos = limit & (~(colLimit | leftLimit | rightLimit));

		int res = 0;
		while (availablePos != 0) {
			int rightMostPos = availablePos & (~availablePos + 1);
			availablePos -= rightMostPos;
			res += process2(limit, colLimit | rightMostPos, (leftLimit | rightMostPos) << 1,
			                (rightLimit | rightMostPos) >> 1);
		}

		return res;
	}
};


int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
	// 汉诺塔问题
	cout << "Hanoi Tower Problem: \n";
	Hanoi::hanoi(3);

	// 打印子字符串问题
	cout << "PrintSubString: \n";
	PrintSubString::printSubString("abc");

	// 打印字符串的全排列
	cout << "PrintAllPermutation abc: \n";
	PrintAllPermutation::printAllPermutation("abc");
	cout << "PrintAllPermutation abca: \n";
	PrintAllPermutation::printAllPermutation("aabc");
	cout << "printAllPermutationWithoutSame abca: \n";
	PrintAllPermutation::printAllPermutationWithoutSame("abca");

	// 抽纸牌问题
	cout << "CardsInLine Winner: " << CardsInLine::cardsInLineWinner({1, 2, 100, 4}) << "\n";

	// 逆序栈
	cout << "Reverse Stack: \n";
	stack<int> st;
	for (int i = 0; i <= 4; i++)
		st.push(i);
	cout << st << "\n";
	ReverseStack::reverseStack(st);
	cout << st << "\n";

	// 字符转化
	cout << "ConvertString 111: " << ConvertString::convertString("111") << "\n";

	// 背包问题
	vector<int> weights{10, 20, 30, 40, 50};
	vector<int> values{50, 120, 150, 210, 240};
	cout << "Knapsack: " << Knapsack::knapsack(weights, values, 50) << "\n";

	// N皇后问题
	cout << "NQueens Problem 1: " << NQueens::nQueens1(8) << "\n";
	cout << "NQueens Problem 2: " << NQueens::nQueens2(8) << "\n";
	return 0;
}
