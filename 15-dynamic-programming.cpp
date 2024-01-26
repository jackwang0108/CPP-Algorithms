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

class ThreeDimensionGridStructuredDP {
public:
	// 前面讲解过CardsInLine这个问题, 现在使用动态规划再次进行求解
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
	static int cardsInLineWinnerRecursion(const vector<int> &vec) {
		if (vec.empty())
			return 0;
		// 玩家A是先手拿牌, 玩家B是后手拿牌
		return std::max(firstHandRecursion(vec, 0, vec.size() - 1), secondHandRecursion(vec, 0, vec.size() - 1));
	}

	// left ~ right 范围内先手拿牌
	static int firstHandRecursion(const vector<int> &vec, int left, int right) {
		// 如果left == right，意味着只剩下一张牌，玩家A直接拿走这张牌。
		if (left == right)
			return vec[right];
		// 玩家A拿走左侧的牌 + 自己下一次抽牌的结果(玩家B会让玩家A下一次抽牌的结果最不好)
		// 玩家A拿走右侧的牌 + 自己下一次抽牌的结果(玩家B会让玩家A下一次抽牌的结果最不好)
		// 两个结果中返回最大的
		return std::max(vec[left] + secondHandRecursion(vec, left + 1, right), vec[right] + secondHandRecursion(vec, left, right - 1));
	}

	static int secondHandRecursion(const vector<int> &vec, int left, int right) {
		// 如果left == right，意味着只剩下一张牌，但玩家B没有机会拿牌，因为玩家A会先拿走它，所以返回0。
		if (left == right)
			return 0;
		// 玩家B抽牌的让玩家A下一次抽牌结果最不利的牌
		return std::min(firstHandRecursion(vec, left + 1, right), firstHandRecursion(vec, left, right - 1));
	}

	// 以 vec = {1, 2, 100, 4}为例
	// 使用动态规划处理这个问题, 这里动态规划的可变参数就是区间, left和right, 也就是两个可变参数
	// 这里firstHand和secondHand函数都是递归的函数, 因此其实有两张表
	// 两个变量left和right的变化范围都是0~N-1, 所以做出来的表如下就是一个正方形的表
	// 首先看边界条件,
	//      a) 可行域: 因为left是要小于right的, 因此表中left > right的区域都是不可行的区域, 也就是表的下三角部分
	//      b) basecase,
	//              1) 对于firstHand, left=right时候直接返回牌的值, 所以对角线就是牌的值
	//              2) 对于secondHand, left=right时候直接返回0, 所以对角线就是0
	// 接下来再分析依赖
	//      a) 主函数返回的是firstHand(0,N)和secondHand(0,N), 所以目标就是两个表的(0,N)处
	//      b) 一般情况下(以(0,N)为例)
	//              1) firstHand(0,N)依赖secondHand(1,3)和secondHand(0,2), 然后取最大
	//              2) secondHand(0,N)依赖firstHand(1,3)和firstHand(0,2), 然后取最小
	// 所以整个表的填表过程就是f表的主对角线填s表的斜上方一行, s表的主对角线填f表的斜上方一行, 以此类推, 直到最后得到两个G值
	//          firstHand表                      secondHand表
	// right    0   1   2   3          right    0   1   2   3
	// left                            left
	//        ,---------------,               ,---------------,
	//  0     | 1 |   |   | G |         0     | 0 |   |   | G |
	//        |---------------|               |---------------|
	//  1     | x | 2 |   |   |         1     | x | 0 |   |   |
	//        |---------------|               |---------------|
	//  2     | x | x |100|   |         2     | x | x | 0 | 0 |
	//        |---------------|               |---------------|
	//  3     | x | x | x | 4 |         3     | x | x | x | 0 |
	//        '---------------'               '---------------'
	static int cardsInLineWinner3DGS(const vector<int> &vec) {
		vector<vector<int>> firstHandDP(vec.size(), vector<int>(vec.size(), 0));
		vector<vector<int>> secondHandDP(vec.size(), vector<int>(vec.size(), 0));
		for (int j = 0; j < vec.size(); j++) {
			firstHandDP[j][j] = vec[j];
			for (int i = j - 1; i >= 0; i--) {
				firstHandDP[i][j] = std::max(vec[i] + secondHandDP[i + 1][j], vec[j] + secondHandDP[i][j - 1]);
				secondHandDP[i][j] = std::min(firstHandDP[i + 1][j], firstHandDP[i][j - 1]);
			}
		}
		return std::max(firstHandDP[0][vec.size() - 1], secondHandDP[0][vec.size() - 1]);
	}

	// 象棋中马的跳法
	// 【题目】
	// 请同学们自行搜索或者想象一个象棋的棋盘，然后把整个棋盘放入第一象限，棋盘的最左下角是(0,0)位置。
	// 那么整个棋盘就是横坐标上9条线、纵坐标上10条线的一个区域。
	// 给你三个参数，x，y，k，返回如果“马”从(0,0)位置出发，必须走k步，最后落在(x,y)上的方法数有多少种
	static int horseJumpRecursion(int x, int y, int k) {
		return processHorseJumpRecursion(x, y, k);
	}

	// 递归的想法很简单, basecase就是越界返回0, 然后stepLeft=0并且两个Pos都是0则返回1
	// 注意这里相当于从(x,y)跳回到(0,0)有多少种跳法
	static int processHorseJumpRecursion(int xPos, int yPos, int stepLeft) {
		if (xPos < 0 || xPos > 8 || yPos < 0 || yPos > 9)
			return 0;
		if (stepLeft == 0)
			return xPos == 0 && yPos == 0 ? 1 : 0;
		return processHorseJumpRecursion(xPos - 2, yPos - 1, stepLeft - 1) +
		       processHorseJumpRecursion(xPos + 2, yPos - 1, stepLeft - 1) +
		       processHorseJumpRecursion(xPos + 2, yPos + 1, stepLeft - 1) +
		       processHorseJumpRecursion(xPos - 2, yPos + 1, stepLeft - 1) +
		       processHorseJumpRecursion(xPos - 1, yPos - 2, stepLeft - 1) +
		       processHorseJumpRecursion(xPos + 1, yPos - 2, stepLeft - 1) +
		       processHorseJumpRecursion(xPos + 1, yPos + 2, stepLeft - 1) +
		       processHorseJumpRecursion(xPos - 1, yPos + 2, stepLeft - 1);
	}

	// 可变变量项x, y, stepLeft(z)一共三个参数, 因此就是一个三维的表
	// 最后返回(x, y, k)这个位置的值
	// 边界条件就是越界就是0, 然后(0,0,0)这个位置的值是1, 其余都是0
	// 每次填表的依赖就是上一层依赖下一层的位置
	static int horseJump3DSG(int x, int y, int k) {
		vector<vector<vector<int>>> dp(k + 1, vector<vector<int>>(9, vector<int>(10, 0)));
		dp[0][0][0] = 1;
		for (int stepLeft = 1; stepLeft <= k; stepLeft++) {
			for (int xPos = 0; xPos < 9; xPos++) {
				for (int yPos = 0; yPos < 10; yPos++) {
					dp[stepLeft][xPos][yPos] += getValue(dp, xPos - 1, yPos + 2, stepLeft - 1);
					dp[stepLeft][xPos][yPos] += getValue(dp, xPos + 1, yPos + 2, stepLeft - 1);
					dp[stepLeft][xPos][yPos] += getValue(dp, xPos - 1, yPos - 2, stepLeft - 1);
					dp[stepLeft][xPos][yPos] += getValue(dp, xPos + 1, yPos - 2, stepLeft - 1);
					dp[stepLeft][xPos][yPos] += getValue(dp, xPos - 2, yPos + 1, stepLeft - 1);
					dp[stepLeft][xPos][yPos] += getValue(dp, xPos + 2, yPos + 1, stepLeft - 1);
					dp[stepLeft][xPos][yPos] += getValue(dp, xPos + 2, yPos - 1, stepLeft - 1);
					dp[stepLeft][xPos][yPos] += getValue(dp, xPos - 2, yPos - 1, stepLeft - 1);
				}
			}
		}
		return dp[k][x][y];
	}

	static int getValue(vector<vector<vector<int>>> &dp, int x, int y, int k) {
		if (x < 0 || x > 8 || y < 0 || y > 9)
			return 0;
		return dp[k][x][y];
	}


	// 再来一个题
	// Bob的生存概率
	// 【题目】
	// 给定五个参数n,m,i,j,k。表示在一个N*M的区域，Bob处在(i,j)点，每次Bob等概率的向上、下、左、右四个方向移动一步，Bob必须走K步。
	// 如果走完之后，Bob还停留在这个区域上，就算Bob存活，否则就算Bob死亡。请求解Bob的生存概率，返回字符串表示分数的方式
	static int bobDieRecursion(int N, int M, int i, int j, int k) {
		return processBobDieRecursion(N, M, i, j, k);
	}

	static int processBobDieRecursion(int N, int M, int i, int j, int k) {
		if (i < 0 || i > N || j < 0 || j > M)
			return 0;
		if (k == 0)
			return 1;

		return processBobDieRecursion(N, M, i - 1, j, k - 1) +
		       processBobDieRecursion(N, M, i + 1, j, k - 1) +
		       processBobDieRecursion(N, M, i, j - 1, k - 1) +
		       processBobDieRecursion(N, M, i, j + 1, k - 1);
	}

	// 改成严格表结构的动态规划, 简单
	// 可变变量三个, i, j, k, 所以是三维表
	// 返回的值是(i, j, k)
	// 边界条件就是越界就是0, 可行域就是N*M内都是1, 所以k从0开始填
	// 三维表中的每一个二位标的范围要注意, 理论上来说整个区域是无限的一个平面
	// 即bob的position的x和y都可能是无穷, 但是除了N*M以外都是0, 所以就可以认为这个范围就是一片淹没在0中的1
	// 因此只需要给1的周围加一圈0即可
	static int bobDie3DGS(int N, int M, int i, int j, int k) {
		vector<vector<vector<int>>> dp(k + 1, vector<vector<int>>(N + 2, vector<int>(M + 2, 0)));
		for (int x = 1; x <= N; x++)
			for (int y = 1; y <= M; y++)
				dp[0][x][y] = 1;
		// 填表
		for (int restStep = 1; restStep <= k; restStep++) {
			for (int x = 1; x <= N; x++) {
				for (int y = 1; y <= M; y++) {
					dp[restStep][x][y] = dp[restStep - 1][x - 1][y];
					dp[restStep][x][y] += dp[restStep - 1][x + 1][y];
					dp[restStep][x][y] += dp[restStep - 1][x][y - 1];
					dp[restStep][x][y] += dp[restStep - 1][x][y + 1];
				}
			}
		}
		return dp[k][i + 1][j + 1];
	}
};

// 在严格表结构的动态规划的基础上, 可以进一步通过观察填表的规律从而进一步改进算法
// 下买以换钱的方法数为例进行讲解
class FinerOptimization {
public:
	// 换钱的方法数
	// 【题目】
	// 给定数组arr，arr中所有的值都为正数且不重复。每个值代表一种面值的货币，每种面值的货币可以使用任意张
	// 再给定一个整数aim，代表要找的钱数，求组成aim的方法数。
	static int coinsWayRecursion(const vector<int> &vec, int aim) {
		return processCoinsWayRecursion(vec, 0, aim);
	}

	static int processCoinsWayRecursion(const vector<int> &vec, int idx, int restAmount) {
		if (idx >= vec.size())
			return restAmount == 0 ? 1 : 0;
		int result = 0;
		for (int numsOfThisOne = 0; numsOfThisOne * vec[idx] <= restAmount; numsOfThisOne++) {
			result += processCoinsWayRecursion(vec, idx + 1, restAmount - numsOfThisOne * vec[idx]);
		}
		return result;
	}


	// 现在分析一下上面的严格表动态规划
	// 以 vec={3, 5, 1, 2}, aim = 10为例
	// 有两个可变变量idx和aim, 取值范围是0~4和0~10, 因此就是一个二维表
	// 然后就是basecase分析,
	//      1) 首先钱是负数, 所以整个表左侧都是不可行的
	//      2) 其次就是idx越界, 即此时已经试完所有的面额了, 如果此时restAmount为0, 就是一个可行解, 否则就是无效解
	//      因此basecase下就是idx=4时最后一行的值
	// 最后就是递归的依赖分析, 即分析递推方程
	// 假设现在在一个通用位置, 例如f(3, 7)
	// 那么现在要计算f(3,7)的值, 即现在尝试的面额的idx=3, 即2元面额, 还剩的钱是7元
	// 那么就需要上面的for循环中的内容, 以2元面额要计算取0张, 1张, 2张....一直到restAmount小于面额
	// 因此, f(3,7)依赖f(4,7), f(4,5), f(4,3), f(4,1), 如下图所示
	// 而计算f(3,5), 即尝试的面额的idx=3, 即2元面额, 还剩5元
	// 同理, f(3,5)依赖f(4,5), f(4,3), f(4,1)
	// = 我们发现, f(3,7)的值完全可以在f(3,5)的基础上加上f(4,7)的值, 这样就可以避免额外的计算
	// = 这样的话, 假设是从下到上, 从左到右的填表,
	// = 计算f(3,7)的解就是f(4,5)的解 + f(4,7)的解, 只需要再进行一次加法计算即可, 不需要像上面一样f(3,7)要计算f(4,7), f(4,5), f(4,3), f(4,1)进行四次加法
	// = 这样做虽然没法减少指标, 但是可以优化常数时间, 因此效果更好
	// ! 所以, 在严格表结构动态规划的基础上, 我们还通过观察, 实现更精细化的动态规划, 减少计算数量
	// ! 这就是为什么需要虽然记忆化搜索动态规划和严格表结构动态规划的时间复杂度都是一样的, 我们还需要进行严格表结构动态规划的原因
	// ! 第一步就是通过递归得到严格表结构的动态规划, 然后再观察严格表结构的填表方式得到规律, 从而得到精细化的动态规划
	// = 需要注意的是, 不是所有观察出来的规律都有实际的含义, 对于上面的这道题, 观察出来的规律的含义就是:
	// =     2元面额还剩7元的解的数量 = 使用一张2元后剩余5元使用2元面额的解的数量 + 不使用2元后剩余7元使用2元面额的解
	// = 更普遍的情况是观察出来的规律是没有实际含义的, 因此就不要纠结寻找规律的实际含义
	//
	// restAmount      0   1   2   3   4   5   6   7   8   9  10
	// idx
	//               ,-------------------------------------------,
	//  0            |   |   |   |   |   |   |   |   |   |   |   |
	//               |-------------------------------------------|
	//  1            |   |   |   |   |   |   |   |   |   |   |   |
	//               |-------------------------------------------|
	//  2            |   |   |   |   |   |   |   |   |   |   |   |
	//               |-------------------------------------------|
	//  3            |   |   |   |   |   | ? |   | ? |   |   |   |
	//               |-------------------------------------------|
	//    f(3,5)依赖  |   | √ |   | √ |   | √ |   |   |   |   |   |
	//    f(3,7)依赖  |   | √ |   | √ |   | √ |   | √ |   |   |   |
	//  4            | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
	//               '-------------------------------------------'
	static int coinsWayDPGSFiner(const vector<int> &vec, int aim) {
		vector<vector<int>> dp(vec.size() + 1, vector<int>(aim + 1, -1));

		// basecase
		dp[vec.size()][0] = 1;
		for (int col = 1; col <= aim; col++)
			dp[vec.size()][col] = -1;

		// 填表
		for (int idx = vec.size() - 1; idx >= 0; idx--) {
			for (int restAmount = 0; restAmount <= aim; restAmount++) {
				// 统一先设置成下方的值
				dp[idx][restAmount] = dp[idx + 1][restAmount];
				// 左边的位置不越界且有效
				if (restAmount - vec[idx] >= 0 && dp[idx][restAmount - vec[idx]] != -1) {
					// 下方的值无效
					if (dp[idx][restAmount] == -1) {
						dp[idx][restAmount] = dp[idx][restAmount - vec[idx]];
					} else
						dp[idx][restAmount] += dp[idx][restAmount - vec[idx]];
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
	cout << "RobotWalkDP GS: " << GridStructuredDP::robotWalkDP(5, 2, 4, 4);
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
	cout << "CoinsMin DP GS: " << GridStructuredDP::coinsMinDPSG(vector<int>{2, 7, 3, 5, 3}, 10);
	std::chrono::time_point t11 = std::chrono::high_resolution_clock::now();
	cout << ", time: " << (t11 - t10).count() << "\n";


	// 抽纸牌问题
	std::chrono::time_point t12 = std::chrono::high_resolution_clock::now();
	cout << "CardsInLine Winner Recursion: " << ThreeDimensionGridStructuredDP::cardsInLineWinnerRecursion({1, 2, 100, 4});
	std::chrono::time_point t13 = std::chrono::high_resolution_clock::now();
	cout << ", time: " << (t13 - t12).count() << "\n";
	std::chrono::time_point t14 = std::chrono::high_resolution_clock::now();
	cout << "CardsInLine Winner 3D GS: " << ThreeDimensionGridStructuredDP::cardsInLineWinner3DGS({1, 2, 100, 4});
	std::chrono::time_point t15 = std::chrono::high_resolution_clock::now();
	cout << ", time: " << (t15 - t14).count() << "\n";


	// 马走日问题
	std::chrono::time_point t16 = std::chrono::high_resolution_clock::now();
	cout << "HorseJump Recursion: " << ThreeDimensionGridStructuredDP::horseJumpRecursion(7, 7, 10);
	std::chrono::time_point t17 = std::chrono::high_resolution_clock::now();
	cout << ", time: " << (t17 - t16).count() << "\n";
	std::chrono::time_point t18 = std::chrono::high_resolution_clock::now();
	cout << "HorseJump 3D GS: " << ThreeDimensionGridStructuredDP::horseJump3DSG(7, 7, 10);
	std::chrono::time_point t19 = std::chrono::high_resolution_clock::now();
	cout << ", time: " << (t19 - t18).count() << "\n";

	// Bob的生存概率
	std::chrono::time_point t20 = std::chrono::high_resolution_clock::now();
	cout << "Bob die Recursion: " << ThreeDimensionGridStructuredDP::bobDieRecursion(10, 10, 3, 2, 5);
	std::chrono::time_point t21 = std::chrono::high_resolution_clock::now();
	cout << ", time: " << (t21 - t20).count() << "\n";
	std::chrono::time_point t22 = std::chrono::high_resolution_clock::now();
	cout << "Bob die 3D GS: " << ThreeDimensionGridStructuredDP::bobDie3DGS(10, 10, 3, 2, 5);
	std::chrono::time_point t23 = std::chrono::high_resolution_clock::now();
	cout << ", time: " << (t23 - t22).count() << "\n";

	// 换钱的方法数
	std::chrono::time_point t24 = std::chrono::high_resolution_clock::now();
	cout << "coinsWay Recursion: " << FinerOptimization::coinsWayRecursion(vector<int>{3, 5, 1, 2}, 10);
	std::chrono::time_point t25 = std::chrono::high_resolution_clock::now();
	cout << ", time: " << (t25 - t24).count() << "\n";
	std::chrono::time_point t26 = std::chrono::high_resolution_clock::now();
	cout << "coinsWay Finer: " << FinerOptimization::coinsWayDPGSFiner(vector<int>{3, 5, 1, 2}, 10);
	std::chrono::time_point t27 = std::chrono::high_resolution_clock::now();
	cout << ", time: " << (t27 - t26).count() << "\n";

	return 0;
}
