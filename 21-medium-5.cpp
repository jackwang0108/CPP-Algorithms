#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>

using std::cout;
using std::map;
using std::priority_queue;
using std::string;
using std::vector;

class RecursionSpeedUp {
	// 当一个递归函数有严格的递归式的时候, 可以加速做到log(N)水平
	// 所谓的严格是指, f(i)严格依赖前项f(i-1), f(i-1), ..., f(0)
	// 与传入数组的第i位的值无关, 即f(i)与vec[i]无关
	//
	// 以斐波那契数列为例, 其递归式为: f(N) = f(N-1) + f(N-2)
	// 递归式中当前项f(N)依赖前两项, 因此存在下述式子 (注意这里是矩阵乘法):
	//  [f(N), f(N-1)] = [f(N-1), f(N-2)] * [a, b]
	//                                      [c, d]
	// 考虑f(1)=1, f(2)=1, f(3)=2, f(4)=3, f(5)=5即可求解得到a=1, b=1, c=1, d=0
	// 因此就得到了通项公式:
	//  [f(N), f(N-1)] = [f(2), f(1)] * ([a, b]) ^ (N - 2)
	//                                  ([c, d])
	// 所以此时问题的关键就在于, 能否快速求得转移矩阵的N次方
public:
	// 对应这个问题, 我们可以考虑如何快速求某一个数的N次方, 例如如何快速求10^75
	// 快速求幂, 首先就是将其指数转换为二进制, 10^75 = 10^1001011 = 10^1000000 * 10^1000 * 10^10 * 10^1
	// 首先设累加和sum=1, t=10
	// 然后首先求t=10^1, 得到指数的第一位, 发现指数第一位是1, 那么就给sum*t
	// 然后求t*=t=10^10, 得到指数第二位, 发现指数第二位是1, 那么就给sum加上t
	// 然后求t*=t=10^100, 得到指数第三位, 发现指数第三位是0, 那么就不给sum加上t
	// 依次类推, 最后就把二进制位上的所有值都计算完了
	// 这样最终得到的时间复杂度就是log(N)
	static int fastPower(int num, int index) {
		int sum = 1;
		while (index > 0) {
			sum *= index & 1 ? num : 0;
			num *= num;
			index >>= 1;
		}
		return sum;
	}

	// 接下来我们再看一下如何快速求一个矩阵的N次方. 例如求: A^75, 那么还是一样的套路
	// 让sum=1(单位矩阵), 然后让t=A
	// 让后t每次和自己乘, 这样就和前面得到的复杂度相同
	// 综上, 快速计算斐波那契数量的方式如下:
	static int fastFibo(int N) {
		if (N < 1)
			return 0;
		if (N == 1 || N == 2)
			return 1;
		vector<vector<int>> base = {{1, 1}, {1, 0}};
		vector<vector<int>> result = matrixPower(base, N - 2);
		return result[0][0] + result[1][0];
	}

	static vector<vector<int>> matrixPower(vector<vector<int>> &matrix, int index) {
		vector<vector<int>> result(matrix.size(), vector(matrix[0].size(), 0));
		for (int i = 0; i < matrix.size(); i++)
			result[i][i] = 1;

		vector<vector<int>> temp = matrix;
		for (; index > 0; index >>= 1) {
			if (index & 1)
				result = matrixMultiply(result, temp);
			temp = matrixMultiply(temp, temp);
		}
		return result;
	}

	static vector<vector<int>> matrixMultiply(vector<vector<int>> &m1, vector<vector<int>> &m2) {
		vector<vector<int>> result(m1.size(), vector(m2[0].size(), 0));
		for (int i = 0; i < m1.size(); i++)
			for (int j = 0; j < m2[0].size(); j++)
				for (int k = 0; k < m2.size(); k++)
					result[i][j] += m1[i][k] * m2[k][j];
		return result;
	}

	// 再来一个例子, 农场养牛问题
	// 农场中有3岁的母牛，开始生小牛, 每年可以生一只，每只牛3年后开始生牛, 且每年都只生一头小头，假定所生的的均为母牛，按照以上牛生牛原则，请问n年后农场有多少牛？
	// 这里的递推式就是f(N) = f(N-1) + f(N-3)
	// 这里f(N)就是今年的数量, f(N-1)就是去年的牛活到现在的数量, f(N-3)就是三年前的牛在今年成熟了, 生的小牛数量
	// 因此这里就是求一个三阶的矩阵即可

	// 再来一个例子, 兔子每年都可以生出来小兔子, 第二年就能继续生兔子, 兔子最多活五年, 之后就会死亡. 求第N年有多少兔子
	// 这里就是f(N)=f(N-1) + 2*f(N-1) + 2*f(N-2) + 2*F(N-3) + 2*f(N-4) - f(N-5)
	// 所以就是一个五阶的矩阵
};

class ZeroLeftOneStringNumber {
	// 字符串只由'0'和'1'两种字符构成
	// 当字符串长度为1时，所有可能的字符串为"0"、"1"；
	// 当字符串长度为2时，所有可能的字符串为"00"、"01"、"10"、"11"；
	// 当字符串长度为3时，所有可能的字符串为"000"、"001"、"010"、"011"、"100"、"101"、"110"、"111"...
	// 如果某一个字符串中，只要是出现'0'的位置，左边就靠着'1'，这样的字符串叫作达标字符串。
	// 给定一个正数N，返回所有长度为N的字符串中，达标字符串的数量。比如，N=3，返回3，因为只有"101"、"110"、"111"达标。

public:
	// 这个题可以直接打表法先观察解的规律, 其实最终的答案就是斐波那契数列
	// 解释为什么是斐波那契数量的话, 就是如果现在确定f(8)的话, 那么第8位一定是1不是0, 否则非法
	// 那么f(8)的答案就是第八位是1的情况下, 剩下七位中f(7)的排列是一定能够使得这总体八位都是合法的
	// 而f(7)的最高位第七位是1, 不是0, 而f(8)的第七位可以是0, 此时就是10______, 剩下六位就是f(6)的排列
	// 因此f(8)的答案就是f(7)+f(6)
	// 这个其实就是先有结果再找解释, 正经做题的时候是没人先想解释在求解的, 都是解出来了再想解释
	static int zeroLeftOneStringNumber(int N) {
		return RecursionSpeedUp::fastFibo(N + 1);
	}

	// 打表
	static void zeroLeftOneStringNumberForce() {
		for (int i = 0; i < 20; i++)
			cout << i << ", " << process("", i, 0) << "\n";
	}

	static int process(string str, int N, int idx) {
		if (idx >= N)
			return 1;
		else if (idx == 0)
			return process("1", N, 1);
		if (str[idx - 1] == '1')
			return process(str + "1", N, idx + 1) + process(str + "0", N, idx + 1);
		else
			return process(str + "1", N, idx + 1);
	}
};

class DeleteWood {
	// 在迷迷糊糊的大草原上，小红捡到了n根木棍，第i根木棍的长度为i
	// 小红现在很开心, 想选出其中的三根木棍组成美丽的三角形。但是小明想捉弄小红，想去掉一些木棍，使得小红任意选三根木棍都不能组成三角形。
	// 请问小明最少去掉多少根木棍呢？给定N，返回至少去掉多少根
public:
	// 分析一下问题, 输入N, 输出N, 这个就是打表得到规律
	// 这里分析一下, 任取a+b<=c(假设c最大), 则a,b最接近c时候这个式子也成立, 那么就没法组成三角形
	// 所以其实就是给定N, 看N前面有多少个斐波那契数列中的数
	static int deleteWood(int N) {
		int last = 1;
		int contains = 0;
		int lastLast = 1;
		while (last <= N) {
			int curr = lastLast + last;
			lastLast = last;
			last = curr;
			contains += 1;
		}
		return N - contains;
	}
};

class CompriseWays {
	// 牛牛准备参加学校组织的春游,出发前牛牛准备往背包里装入一些零食,牛牛的背包容量为w。牛牛家里一共有n袋零食,第i袋零食体积为v[i]。牛牛想知道在总体积不超过背包容量的情况下，他一共有多少种零食放法(总体积为0也算一种放法)。
public:
	// 背包问题求得是最后背包用完, 所以返回的是dp[v.size()-1][w], 这里要返回总体积不超过背包容量
	// 所以其实返回dp[0][w] + dp[1][w] + ... + dp[v.size() - 1][w]即可
	static int compriseWays(const vector<int> &v, int w) {
		return processCompriseWays(v, 0, w);
	}

	static int processCompriseWays(const vector<int> &v, int idx, int wLeft) {
		if (idx >= v.size() && wLeft >= 0)
			return 1;
		if (wLeft < 0)
			return 0;
		int takeThisOne = processCompriseWays(v, idx + 1, wLeft - v[idx]);
		int dontTakeThisOne = processCompriseWays(v, idx + 1, wLeft);
		return takeThisOne + dontTakeThisOne;
	}
};
class ChooseWork {
	// 为了找到自己满意的工作，牛牛收集了每种工作的难度和报酬。
	// 牛牛选工作的标准是在难度不超过自身能力值的情况下，牛牛选择报酬最高的工作。
	// 在牛牛选定了自己的工作后，牛牛的小伙伴们来找牛牛帮忙选工作，牛牛依然使用自己的标准来帮助小伙伴们。牛牛的小伙伴太多了，于是他只好把这个任务交给了你。
	// class Job {
	// public:
	//     int money; //该工作的报酬
	//     int hard;  //该工作的难度
	//     Job(int money, int hard) {
	//         money = money;
	//         hard=hard;
	//     }
	// }
	// 给定一个Job类型的数组jobArr，表示所有的工作。给定一个int类型的数组arr，表示所有小伙伴的能力。返回int类型的数组，表示每一个小伙伴按照牛牛的标准选工作后所能获得的报酬。

public:
	struct Job {
		int hard;
		int money;
	};

	// 这题确实没必要, 同样难度的工作保留报酬最高的哪一个即可, 后面就是按照难度降序排列即可
	static vector<int> chooseWork(const vector<Job> &vec, vector<int> arr) {
		std::function<bool(Job &, Job &)> hardComparator = [](Job &j1, Job &j2) -> bool {
			return j1.hard < j2.hard;
		};
		priority_queue<Job> que;

		return {0};
	}
};

int main(int argc, char *argv[]) {
	// 严格递归式加速
	cout << "Fast Power: " << RecursionSpeedUp::fastPower(10, 3) << "\n";
	cout << "Fast Fibo: " << RecursionSpeedUp::fastFibo(10) << "\n";

	// 达标字符串
	// cout << "ZeroLeftOneStringNumber Force: ";
	// ZeroLeftOneStringNumber::zeroLeftOneStringNumberForce();
	cout << "ZeroLeftOneStringNumber: " << ZeroLeftOneStringNumber::zeroLeftOneStringNumber(5) << "\n";

	// 删木棍问题
	cout << "Delete Wood: " << DeleteWood::deleteWood(20) << "\n";

	// 放零食问题
	cout << "Comprise Ways: " << CompriseWays::compriseWays({3, 2, 5, 2, 3}, 10) << "\n";
	return 0;
}
