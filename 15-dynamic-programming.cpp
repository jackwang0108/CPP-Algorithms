#include <chrono>
#include <iostream>
#include <vector>


using std::cout;
using std::vector;


// 在前面讲暴力递归那节课的时候我们说过, 分支限界, 回溯, 以及动态规划本质上都是暴力递归
// 只不过在暴力递归的基础上进行了改进. 例如分支限界就是在递归的过程中删去一些不满足条件的分支
// 从而减少递归搜索的路径, 加速问题求解

// 我们还说过, 暴力递归转化为动态规划是有固定套路的, 所以本节就将讲解如何进行转化.
// = 整个流程如下:
//  1. 首先通过尝试找出递归方法 (接下来的优化就和递归的含义没关系了, 只需要递归的结构就可以改出来动态规划了)
//  2. 对递归进行修改就可以得到记忆化搜索的动态规划, 这个是非常简单的 (有固定的策略)
//  3. 改出来记忆化搜索的动态规划之后, 还可以继续优化得到严格表结构的动态规划. 记忆化搜索的动态规划不会对
//     对各个状态之间的依赖整理出来, 但是严格表结构的动态规划会进行一个整理. 对大多数问题, 记忆化搜索动态规划
//     和严格表结构动态规划时间复杂度是一样的, 但是为什么还要严格表结构的动态规划?
//     因为在某些问题上, 还可以进一步优化, 而进一步优化需要我们把依赖顺序要搞清楚, 也就是先搞出来严格表结构动态规划
//  4. 在严格表结构动态规划的基础上, 我们可以进行四边形不等式优化, 斜率优化等等, 从而得到一个更精致的严格表结构动态规划
// = 中间的整个优化过程其实已经和业务没有关系了, 也就是说, 我们看到了一个递归之后, 完全不需要知道题中说的是什么, 就可以
// = 把它改成一个动态规划问题
// 接下来针对每个点, 我们都会给出例题进行讲解

class MemorizedSearchingDP {
public:
	// 机器人移动问题
	// 给你几个参数：
	//  N：代表总共有多少个位置。
	//  S：机器人的出发位置。
	//  E：机器人的目的地。
	//  K：机器人必须走K步。
	// 问题：请你求出机器人从S起始位置走K步到达终止位置E，一共有多少种走法？注意，每次机器人必须走一步，不能留在原地，每次只能走一格，可以向右或者向左。
	// 时间复杂度是O(2^k), 因此递归就是一个二叉树展开, 高度为K, 所以时间复杂度O(2^k)
	static int robotWalkRecursion(int n, int s, int e, int k) {
		return processRobotWalkRecursion(n, e, s, k);
	}

	// 暴力递归版, 很好想, 每次要么向左要么向右, 终止条件就是走的超出范围或者步数到了, 如果恰好到了, 那么就返回1
	static int processRobotWalkRecursion(int n, int e, int currStart, int leftWalk) {
		if (leftWalk == 0)
			return currStart == e ? 1 : 0;
		if (currStart == 1)
			return processRobotWalkRecursion(n, e, currStart + 1, leftWalk - 1);
		if (currStart == n)
			return processRobotWalkRecursion(n, e, currStart - 1, leftWalk - 1);
		return processRobotWalkRecursion(n, e, currStart + 1, leftWalk - 1) + processRobotWalkRecursion(n, e, currStart - 1, leftWalk - 1);
	}


	// = 整个暴力递归的过程中, 可变的参数只有两个: currStart和leftWalk, 如果这两个参数定了, 那么函数的返回值就定了
	// 而处理 f(2,4)的时候会处理f(1,3) -> f(2,2) -> ..., 还会处理f(3,3)->f(2,2)->...
	// 因此f(2,2)会被重复计算两边, 因此如果在处理的时候有一个缓存能够记录下来处理过的值, 那么就可以加速暴力递归的过程
	// 而因为递归的时候有两个可变参数, 所以这里的缓存自然就是一个二维表
	// = 注意, 核心问题是当前的选择不会对后续的选择产生影响, 也就是说f(3,3)得到的f(2,2)和f(1,3)得到的f(2,2)的答案是一样的
	// = 这被称为无后效性. 如果有后效性, 例如前面的讲的抽纸牌, 那么就需要修改递归的方式, 使其成为无后效性的递归
	// = 面试的时候考我们的一定是无后效性的, 因为要考出我们的水平, 如果写出来的递归是有后效性的, 那就是我们实现的问题了, 此时就需要修改出来一个没有后效性的递归
	// 时间复杂度O(k*n), 因为最坏情况下需要把整个表填满
	static int robotWalkDP(int n, int s, int e, int k) {
		// 确定缓存的大小就看可变参数的返回, 这里leftWalk的返回是0~k, 所以大小就是0~k+1
		// 而currStart的范围是1~n, 所以大小就是0~n+1
		// 初始化为-1, 做个标记
		vector<vector<int>> dp(k + 1, vector<int>(n + 1, -1));
		return processRobotWalkDP(n, e, s, k, dp);
	}

	static int processRobotWalkDP(int n, int e, int currStart, int leftWalk, vector<vector<int>> &dp) {
		if (dp[leftWalk][currStart] != -1)
			return dp[leftWalk][currStart];
		if (leftWalk == 0) {
			dp[leftWalk][currStart] = currStart == e ? 1 : 0;
			return dp[leftWalk][currStart];
		}
		if (currStart == 1) {
			dp[leftWalk][currStart] = processRobotWalkDP(n, e, currStart + 1, leftWalk - 1, dp);
			return dp[leftWalk][currStart];
		}
		if (currStart == n) {
			dp[leftWalk][currStart] = processRobotWalkDP(n, e, currStart - 1, leftWalk - 1, dp);
			return dp[leftWalk][currStart];
		}
		dp[leftWalk][currStart] = processRobotWalkDP(n, e, currStart - 1, leftWalk - 1, dp) + processRobotWalkDP(n, e, currStart + 1, leftWalk - 1, dp);
		return dp[leftWalk][currStart];
	}
};


class GridStructuredDP {
public:
	// 同样是针对上面机器人移动的问题, 针对输入N=5, S=2, E=4, K=4分析依赖
	// = 整个DP表的结构如下, 要求计算的就是表中的(4,2)位置的值:
	// k\n  0   1   2   3   4   5
	//    ,-----------------------,
	//  0 |   |   |   |   |   |   |
	//    |-----------------------|
	//  1 |   |   |   |   |   |   |
	//    |-----------------------|
	//  2 |   |   |   |   |   |   |
	//    |-----------------------|
	//  3 |   |   |   |   |   |   |
	//    |-----------------------|
	//  4 |   |   | G |   |   |   |
	//    '-----------------------'
	// = 首先是什么情况下可以直接给出答案? 那就是basecase的时候可以直接给答案, 在k=0的时候可以直接给答案, 所以填表的时候答案就如下
	// k\n  0   1   2   3   4   5
	//    ,-----------------------,
	//  0 |   | 0 | 0 | 0 | 1 | 0 |
	//    |-----------------------|
	//  1 |   |   |   |   |   |   |
	//    |-----------------------|
	//  2 |   |   |   |   |   |   |
	//    |-----------------------|
	//  3 |   |   |   |   |   |   |
	//    |-----------------------|
	//  4 |   |   | G |   |   |   |
	//    '-----------------------'
	// = 然后分析边角料, 因为N取值范围取不到0, 所以左边都是不可能的值
	// k\n  0   1   2   3   4   5
	//    ,-----------------------,
	//  0 | x | 0 | 0 | 0 | 1 | 0 |
	//    |-----------------------|
	//  1 | x |   |   |   |   |   |
	//    |-----------------------|
	//  2 | x |   |   |   |   |   |
	//    |-----------------------|
	//  3 | x |   |   |   |   |   |
	//    |-----------------------|
	//  4 | x |   | G |   |   |   |
	//    '-----------------------'
	// = 然后分析两侧, currStart=1的时候要依赖f(currStart + 1, leftWalk - 1)的值, 所以就是右上角的值
	// = 同理currStart=N的时候要依赖f(currStart - 1, leftWalk - 1)值, 所以就是右上角的值
	// k\n  0   1   2   3   4   5
	//    ,-----------------------,
	//  0 | x | 0 | 0 | 0 | 1 | 0 |
	//    |-----------------------|
	//  1 | x | ↗️ |   |   |   | ↖ |
	//    |-----------------------|
	//  2 | x | ↗️ |   |   |   | ↖ |
	//    |-----------------------|
	//  3 | x | ↗️️️️ |   |   |   | ↖ |
	//    |-----------------------|
	//  4 | x |   | G |   |   | ↖ |
	//    '-----------------------'
	// = 然后在分析中间的, 中间时候currStart依赖f(currStart+1, leftWalk-1)和f(currStart-1, leftWalk-1)
	// = 中间两列同理, 这里略掉了
	// k\n  0   1   2   3   4   5
	//    ,-----------------------,
	//  0 | x | 0 | 0 | 0 | 1 | 0 |
	//    |-----------------------|
	//  1 | x | ↗️ |   |↖ ↗|   | ↖ |
	//    |-----------------------|
	//  2 | x | ↗️ |   |↖ ↗|   | ↖ |
	//    |-----------------------|
	//  3 | x | ↗️️️️ |   |↖ ↗|   | ↖ |
	//    |-----------------------|
	//  4 | x |   | G |↖ ↗|   | ↖ |
	//    '-----------------------'
	// = 那么填表的时候, 就可以直接给出答案
	// k\n  0   1   2   3   4   5          k\n  0   1   2   3   4   5
	//    ,-----------------------,           ,-----------------------,
	//  0 | x | 0 | 0 | 0 | 1 | 0 |         0 | x | 0 | 0 | 0 | 1 | 0 |
	//    |-----------------------|           |-----------------------|
	//  1 | x | 0 | 0 | 1 | 0 | 1 |         1 | x | 0 | 0 | 1 | 0 | 1 |
	//    |-----------------------|  填第2行   |-----------------------|    同理
	//  2 | x | ↗️ |   |↖ ↗|   | ↖ | ------> 2 | x | 0 | 1 | 0 | 2 | 0 |  -------> ...
	//    |-----------------------|           |-----------------------|
	//  3 | x | ↗️️️️ |   |↖ ↗|   | ↖ |         3 | x | ↗️️️️ |   |↖ ↗|   | ↖ |
	//    |-----------------------|           |-----------------------|
	//  4 | x |   | G |↖ ↗|   | ↖ |         4 | x |   | G |↖ ↗|   | ↖ |
	//    '-----------------------'           '-----------------------'
	// = 最后就得到答案了
	// k\n  0   1   2   3   4   5
	//    ,-----------------------,
	//  0 | x | 0 | 0 | 0 | 1 | 0 |
	//    |-----------------------|
	//  1 | x | 0 | 0 | 1 | 0 | 1 |
	//    |-----------------------|
	//  2 | x | 0 | 1 | 0 | 2 | 0 |
	//    |-----------------------|
	//  3 | x | 1 | 0 | 3 | 0 | 2 |
	//    |-----------------------|
	//  4 | x | 0 | 4 | 0 | 5 | 0 |
	//    '-----------------------'
	// = 这种方式就是严格表结构的动态规划, 他和前面的记忆化搜索的动态规划是不同的, 表结构的动态规划是要纠结依赖的
	// = 只不过针对机器人运动这个问题, 时间复杂度上是一样的
	// = 所以严格表结构的方法, 就是根据依赖关系填表, 然后返回
	static int robotWalkDP(int n, int s, int e, int k) {
		vector<vector<int>> dp(k + 1, vector<int>(n + 1, -1));
		for (int colIdx = 1; colIdx < dp[0].size(); colIdx++)
			dp[0][colIdx] = 0;
		dp[0][e] = 1;

		for (int rowIdx = 1; rowIdx < dp.size(); rowIdx++) {
			for (int colIdx = 1; colIdx < dp[rowIdx].size(); colIdx++) {
				if (colIdx == 1)
					dp[rowIdx][colIdx] = dp[rowIdx - 1][colIdx + 1];
				else if (colIdx == dp[rowIdx].size() - 1)
					dp[rowIdx][colIdx] = dp[rowIdx - 1][colIdx - 1];
				else
					dp[rowIdx][colIdx] = dp[rowIdx - 1][colIdx - 1] + dp[rowIdx - 1][colIdx + 1];
			}
		}
		return dp[k][s];
	}


	// 下面我们再看一个问题, 再来实践一把

	// 换钱的最少货币数
	// 【题目】
	// 给定数组arr，arr中所有的值都为正数且不重复。每个值代表一种面值的货币，每种面值的货币可以使用一张，再给定一个整数aim，代表要找的钱数，求组成aim的最少货币数。
	// 首先是暴力递归的方法, 就是要这个钱, 然后看后面的, 然后再看不要这个钱, 看后面的
	static int coinsMinRecursion(const vector<int> &coins, int aim) {
		std::pair<bool, int> result = processCoinsMinRecursion(coins, aim, 0);
		return result.first ? result.second : -1;
	}

	static std::pair<bool, int> processCoinsMinRecursion(const vector<int> &coins, int restAmount, int thisOneIndex) {
		if (restAmount < 0)
			return {false, -1};
		if (restAmount == 0)
			return {true, 0};
		if (thisOneIndex >= coins.size())
			return {false, -1};
		std::pair<bool, int> getThisOne = processCoinsMinRecursion(coins, restAmount - coins[thisOneIndex], thisOneIndex + 1);
		std::pair<bool, int> dontGetThisOne = processCoinsMinRecursion(coins, restAmount, thisOneIndex + 1);
		if (getThisOne.first)
			getThisOne.second += 1;
		if (getThisOne.first && !dontGetThisOne.first)
			return getThisOne;
		else if (!getThisOne.first && dontGetThisOne.first)
			return dontGetThisOne;
		else if (!getThisOne.first && !dontGetThisOne.first)
			return {false, -1};
		else
			return getThisOne.second < dontGetThisOne.second ? getThisOne : dontGetThisOne;
	}

	// 记忆化搜索 DP
	static int coinsMinDPMS(const vector<int> &coins, int aim) {
		// -2 表示还没有搜索过, -1 表示不是可行解
		vector<vector<int>> dp(coins.size() + 1, vector<int>(aim + 1, -2));
		return processCoinsMinDP(coins, aim, 0, dp);
	}

	static int processCoinsMinDP(const vector<int> &coins, int restAmount, int thisCoinIndex, vector<vector<int>> &dp) {
		if (restAmount < 0)
			return -1;
		if (restAmount == 0)
			return 0;
		if (thisCoinIndex == coins.size())
			dp[thisCoinIndex][restAmount] = -1;
		if (dp[thisCoinIndex][restAmount] != -2)
			return dp[thisCoinIndex][restAmount];
		int getThisOne = processCoinsMinDP(coins, restAmount - coins[thisCoinIndex], thisCoinIndex + 1, dp);
		int dontGetThisOne = processCoinsMinDP(coins, restAmount, thisCoinIndex + 1, dp);
		if (getThisOne == -1 && dontGetThisOne == -1)
			dp[thisCoinIndex][restAmount] = -1;
		else {
			if (getThisOne == -1)
				dp[thisCoinIndex][restAmount] = dontGetThisOne;
			else if (dontGetThisOne == -1)
				dp[thisCoinIndex][restAmount] = getThisOne + 1;
			else
				dp[thisCoinIndex][restAmount] = std::min(getThisOne + 1, dontGetThisOne);
		}
		return dp[thisCoinIndex][restAmount];
	}

	// coins = vector<int>{2, 7, 3, 5, 3}, aim = 10为例
	// 严格表结构填写:
	//      1. 首先确定需要寻找的终止位置, 根据主函数的返回, 终止位置就是f(0,10)
	//      2. 然后填basecase
	//          a. 就是数组越界
	//          b. restAmount==0
	//          c. 没有硬币了, 并且 restAmount > 0
	//         所以填表的时候首先restAmount=0这一列都是0, 其次thisCoinIndex=coins.size()这一行(除了第一列)都是-1
	// restAmount      0    1    2    3    4    5    6    7    8    9   10
	//thisCoinIndex
	//              ,------------------------------------------------------,
	//  0           |  0 | -2 | -2 | -2 | -2 | -2 | -2 | -2 | -2 | -2 | -2 |
	//              |------------------------------------------------------|
	//  1           |  0 | -2 | -2 | -2 | -2 | -2 | -2 | -2 | -2 | -2 | -2 |
	//              |------------------------------------------------------|
	//  2           |  0 | -2 | -2 | -2 | -2 | -2 | -2 | -2 | -2 | -2 | -2 |
	//              |------------------------------------------------------|
	//  3           |  0 | -2 | -2 | -2 | -2 | -2 | -2 | -2 | -2 | -2 | -2 |
	//              |------------------------------------------------------|
	//  4           |  0 | -2 | -2 | -2 | -2 | -2 | -2 | -2 | -2 | -2 | -2 |
	//              |------------------------------------------------------|
	//  5           |  0 | -1 | -1 | -1 | -1 | -1 | -1 | -1 | -1 | -1 | -1 |
	//              '------------------------------------------------------'
	//      3. 接下来确定依赖关系
	//          例如这里的f(2, 5), 也就是说对于第2个coin, 余额为5的情况下
	//          那么就是要第2个coin(编号从0开始), 则依赖拿这个硬币, 余额为3和不拿这个硬币, 余额为5, 即依赖f(3, 2)和f(3, 5)
	//          所以依赖关系就是自己的正下方和下方一行余额减去自己的面值的那一列
	//      4. 接下来按照顺序填写, 所以这里就是从下到上, 从左到右依次填写
	static int coinsMinDPSG(const vector<int> &coins, int aim) {
		vector<vector<int>> dp(coins.size() + 1, vector<int>(aim + 1, -2));
		for (int thisCoinIndex = 0; thisCoinIndex <= coins.size(); thisCoinIndex++)
			dp[thisCoinIndex][0] = 0;
		for (int restAmount = 1; restAmount <= aim; restAmount++)
			dp[coins.size()][restAmount] = -1;

		for (int thisCoinIndex = coins.size() - 1; thisCoinIndex >= 0; thisCoinIndex--) {
			for (int restAmount = 1; restAmount <= aim; restAmount++) {
				int getThisOne = -1;
				int dontGetThisOne = dp[thisCoinIndex + 1][restAmount];
				if (restAmount >= coins[thisCoinIndex])
					getThisOne = dp[thisCoinIndex + 1][restAmount - coins[thisCoinIndex]];
				if (dontGetThisOne == -1 && getThisOne == -1)
					dp[thisCoinIndex][restAmount] = -1;
				else {
					if (getThisOne == -1)
						dp[thisCoinIndex][restAmount] = dontGetThisOne;
					else if (dontGetThisOne == -1)
						dp[thisCoinIndex][restAmount] = getThisOne + 1;
					else
						dp[thisCoinIndex][restAmount] = std::min(getThisOne + 1, dontGetThisOne);
				}
			}
		}
		return dp[0][aim];
	}
};


int main(int argc, char *argv[]) {
	// 机器人移动问题
	// 暴力递归版
	std::chrono::time_point t0 = std::chrono::high_resolution_clock::now();
	cout << "RobotWalkRecursion: " << MemorizedSearchingDP::robotWalkRecursion(5, 2, 4, 4);
	std::chrono::time_point t1 = std::chrono::high_resolution_clock::now();
	cout << ", time=" << (t1 - t0).count() << "\n";
	// 记忆化搜索DP
	std::chrono::time_point t2 = std::chrono::high_resolution_clock::now();
	cout << "RobotWalkDP MS: " << MemorizedSearchingDP::robotWalkDP(5, 2, 4, 4);
	std::chrono::time_point t3 = std::chrono::high_resolution_clock::now();
	cout << ", time=" << (t3 - t2).count() << "\n";
	// 严格表结构DP
	std::chrono::time_point t4 = std::chrono::high_resolution_clock::now();
	cout << "RobotWalkDP SG: " << GridStructuredDP::robotWalkDP(5, 2, 4, 4);
	std::chrono::time_point t5 = std::chrono::high_resolution_clock::now();
	cout << ", time=" << (t5 - t4).count() << "\n";

	// 换钱问题
	// 暴力递归
	std::chrono::time_point t6 = std::chrono::high_resolution_clock::now();
	cout << "CoinsMin Recursion: " << GridStructuredDP::coinsMinRecursion(vector<int>{2, 7, 3, 5, 3}, 10);
	std::chrono::time_point t7 = std::chrono::high_resolution_clock::now();
	cout << ", time: " << (t7 - t6).count() << "\n";
	// 记忆化搜索DP
	std::chrono::time_point t8 = std::chrono::high_resolution_clock::now();
	cout << "CoinsMin DP MS: " << GridStructuredDP::coinsMinDPMS(vector<int>{2, 7, 3, 5, 3}, 10);
	std::chrono::time_point t9 = std::chrono::high_resolution_clock::now();
	cout << ", time: " << (t9 - t8).count() << "\n";
	// 严格表结构DP
	std::chrono::time_point t10 = std::chrono::high_resolution_clock::now();
	cout << "CoinsMin DP SG: " << GridStructuredDP::coinsMinDPSG(vector<int>{2, 7, 3, 5, 3}, 10);
	std::chrono::time_point t11 = std::chrono::high_resolution_clock::now();
	cout << ", time: " << (t11 - t10).count() << "\n";
	return 0;
}