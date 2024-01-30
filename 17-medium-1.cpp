#include <iostream>
#include <limits>
#include <random>
#include <string>
#include <vector>

using std::cout;
using std::string;
using std::vector;


// Sliding Window
class CordCoverMaxPoint {
	// 给定一个有序数组arr，代表数轴上从左到右有n个点arr[0]、arr[1]...arr[n－1]
	// 给定一个正数L，代表一根长度为L的绳子
	// 求绳子最多能覆盖其中的几个点
public:
	// 解法一: 计算右边界左扩之后的值, 然后再数组中寻找右侧
	// 时间复杂度O(N^2), 更快的查找方法就是二分
	static int cordCoverMaxPoint(const vector<int> &vec, int L) {
		int maxPoints = 0;
		for (int i = 0; i < vec.size(); i++) {
			int currMax = 0;
			int leftMostValue = vec[i] - L;
			for (int j = 0; j <= i; j++) {
				// 利用有序
				if (vec[j] >= leftMostValue) {
					currMax = i - j + 1;
					break;
				}
			}
			maxPoints = std::max(maxPoints, currMax);
		}
		return maxPoints;
	}

	// 使用滑动窗口因为每个点都只会被走过一次, 所以时间复杂度就是O(N)
	static int cordCoverMaxPointSlidingWindow(const vector<int> &vec, int L) {
		int maxPoints = 0, currPoints = 0;
		int leftIdx = 0, rightIdx = 0;
		while (rightIdx <= vec.size()) {
			if (vec[rightIdx] - vec[leftIdx] > L)
				leftIdx++, maxPoints = std::max(maxPoints, currPoints), currPoints = 0;
			else
				rightIdx++, currPoints++;
		}
		return maxPoints;
	}
};


// 暴力打表
class AppleMinBags {
	// 小虎去附近的商店买苹果，奸诈的商贩使用了捆绑交易，只提供6个每袋和8个每袋的包装包装不可拆分。
	// 可是小虎现在只想购买恰好n个苹果，小虎想购买尽量少的袋数方便携带。如果不能购买恰好n个苹果，小虎将不会购买。
	// 输入一个整数n，表示小虎想购买的个苹果，返回最小使用多少袋子。如果无论如何都不能正好装下，返回-1。
public:
	// 贪心策略, 上来先取8个袋子的最大, 然后再确定6个的袋子需要多少个
	static int appleMinBags(int n) {
		int bag6 = -1;
		int bag8 = n / 8;
		int rest = n - bag8 * 8;
		while (bag8 >= 0 && rest < 24) {
			int restUse6 = rest % 6 == 0 ? rest / 6 : -1;
			if (restUse6 != -1) {
				bag6 = restUse6;
				break;
			}
			rest = n - (--bag8 * 8);
		}
		return bag6 == -1 ? -1 : bag6 + bag8;
	}

	// 事实上, 对于输入为int, 输出为int的题目, 可以通过观察输出的规律进行暴力打表
	// 因为这类题目通常背后会有数学原理, 因此通过暴力打表之后就可以反向通过结果推算答案的规律
	// 至于中间的数学证明? 略
	static void appleMinBagsTest() {
		for (int i = 0; i < 100; i++)
			cout << i << ": " << appleMinBags(i) << "\n";
	}

	// 总结打表得到的规律
	static int appleMinBagsAwesome(int n) {
		if ((n & 1) != 0)
			return -1;
		if (n < 18)
			return n == 0 ? 0 : ((n == 6 || n == 8) ? 1 : ((n == 12 || n == 14 || n == 16) ? 2 : -1));
		return (n - 18) / 8 + 3;
	}
};


// 暴力打表
class EatGrass {
	// 牛牛和羊羊都很喜欢青草。今天他们决定玩青草游戏。
	// 最初有一个装有n份青草的箱子,牛牛和羊羊依次进行,牛牛先开始。
	// 在每个回合中,每个玩家必须吃一些箱子中的青草,所吃的青草份数必须是4的x次幂,比如1,4,16,64等等。
	// 谁先吃完谁赢, 如果输入0份的话那就是羊赢。假定牛牛和羊羊都是按照最佳方法进行游戏,请输出胜利者的名字。
public:
	// 递归的解法, basecase就是剩0~4份草, 然后剩余a份草之后, 就从1,4,..,4^k(k<log(4,a))依次吃, 如果有一次自己能赢那就赢了
	static string winner(int n) {
		// basecase:
		// 0  1  2  3  4
		// 羊 牛 羊  牛 牛
		if (n < 5)
			return (n == 0 || n == 2) ? "后手" : "先手";
		// 先手吃草
		int eat = 1;
		while (eat <= n) {
			// n - eat表示现在自己吃了, 然后就轮到后手吃了
			// 子过程表示后手吃, 所以如果子过程后手吃返回返回了后手赢, 则就是后手的后手赢了, 所以在母过程中就是自己赢了
			if (winner(n - eat) == "后手")
				return "先手";
			// 防止溢出
			if (eat > n / 4)
				break;
			eat *= 4;
		}
		return "后手";
	}

	// 打表吧, 观察规律, 直接出答案
	static void winnerTest() {
		for (int i = 0; i < 100; i++)
			cout << i << ": " << winner(i) << " " << winnerAwesome(i) << "\n";
	}

	// 观察出的结果就是后先后先先的规律
	static string winnerAwesome(int n) {
		n = n % 5;
		if (n == 0 || n == 2)
			return "后手";
		else
			return "先手";
	}
};


// 辅助数据结构加速
class ColorLeftRight {
	// 牛牛有一些排成一行的正方形。每个正方形已经被染成红色或者绿色。
	// 牛牛现在可以选择任意一个正方形然后用这两种颜色的任意一种进行染色,这个正方形的颜色将会被覆盖。
	// 牛牛的目标是在完成染色之后,每个红色R都比每个绿色G距离最左侧远。牛牛想知道他最少需要涂染几个正方形。如样例所示:s=RGRGR
	// 我们涂染之后变成GGRRR满足要求了,涂染的个数为2,没有比这个更好的涂染方案。
public:
	// 实际上就是要求染色之后, 左侧都是G, 右侧都是R, 然后求染色最少的次数
	static int colorLeftRight(const string &str) {
		int min = std::numeric_limits<int>::max();
		for (int leftPos = 0; leftPos <= str.length(); leftPos++) {
			int curr = 0;
			for (int i = 0; i < leftPos; i++)
				if (str[i] == 'R')
					curr++;
			for (int j = leftPos; j < str.size(); j++)
				if (str[j] == 'G')
					curr++;
			min = std::min(curr, min);
		}
		return min;
	}

	// 实际上可以利用辅助的数据结构进行优化.
	// 这里就是利用左侧有几个R, 右侧有几个G即可
	// 未来求答案就是直接去查询即可
	static int colorLeftRight2(const string &str) {
		vector<int> rightG(str.length(), 0);
		rightG[str.length() - 1] = str.back() == 'G' ? 1 : 0;
		for (int i = str.length() - 2; i >= 0; i--)
			rightG[i] = rightG[i + 1] + (str[i] == 'G' ? 1 : 0);

		int currR = 0;
		int min = std::numeric_limits<int>::max();
		for (int i = 0; i < str.length() - 1; i++) {
			currR += str[i] == 'R' ? 1 : 0;
			min = std::min(min, currR + rightG[i + 1]);
		}
		return std::min(min, currR + (str.back() == 'R' ? 1 : 0));
	}
};

class MaxOneBorderSize {
	// 给定一个N*N的矩阵matrix，只有0和1两种值，返回边框全是1的最大正方形的边长长度。
	// 例如:
	// 01111
	// 01001
	// 01001
	// 01111
	// 01011
	// 其中边框全是1的最大正方形的大小为4*4，所以返回4。
public:
	// 考虑这个问题需要首先考虑如何遍历
	// 首先是所有的子矩阵数量, 即长方形数量. 因为一个长方形可以由左上角点和右下角点唯一确定, 因此所有的子矩阵数量就是O(N^4)数量级, 可能会有重复, 例如左上角和右下角互换, 但是数量级还是这么多
	// 然后再考虑正方形数量, 左上角还是O(N^2)得可能, 然后对角线长度有O(N)种可能, 所以正方形子矩阵就有O(N^3)的可能
	// 此外还要检测每个边都是1, 所以时间复杂度就是O(N^4)
	static int maxOneBorderSize(vector<vector<int>> &matrix) {
		int numRows = matrix.size();
		int numCols = matrix[0].size();

		for (int row = 0; row < numRows; row++) {
			for (int col = 0; col < numCols; col++) {
				// 对角线不能超标
				for (int diag = 0; diag <= std::min(numRows - row, numCols - col); diag++) {
					// 然后遍历四条边, 检测边是否都是1
					for (int upper = 0; upper <= row + diag; upper++)
						// 检测上边
						;
					for (int bottom = 0; bottom <= row + diag; bottom++)
						// 检测下边
						;
					for (int left = 0; left <= col + diag; left++)
						// 检测左边
						;
					for (int right = 0; right <= col + diag; right++)
						// 检测右边
						;
				}
			}
		}
		return -1;
	}

	// 如何进行预处理, 使用额外的数据结构来加速每个边的判定?
	// 因为核心就是验证边上是否都是连续的1, 因此可以提前构建连续1的个数的辅助数组
	static int maxOneBorderSizeAwesome(vector<vector<int>> &matrix) {
		int numRows = matrix.size();
		int numCols = matrix[0].size();

		// 预处理, 获得辅助数组
		// 每个点包含自己在内右侧有几个连续的1
		vector<vector<int>> helperRight(matrix.size() + 1, vector<int>(matrix[0].size() + 1, 0));
		// 每个点包含自己在内下侧有几个连续的1
		vector<vector<int>> helperDown(matrix.size() + 1, vector<int>(matrix[0].size() + 1, 0));
		// 预处理得到辅助数组
		for (int row = matrix.size() - 1; row >= 0; row--) {
			for (int col = matrix[row].size() - 1; col >= 0; col--) {
				helperRight[row][col] = matrix[row][col] == 0 ? 0 : (1 + helperRight[row][col + 1]);
				helperDown[row][col] = matrix[row][col] == 0 ? 0 : (1 + helperDown[row + 1][col]);
			}
		}

		int maxBorderSize = 0;
		for (int row = 0; row < matrix.size(); row++) {
			for (int col = 0; col < matrix[0].size(); col++) {
				for (int diag = 1; diag <= std::min(numRows - row, numCols - col); diag++) {
					// 判断的时候直接取点出来看连续1的数量是否够diag个
					if (helperRight[row][col] >= diag && helperDown[row][col] >= diag && helperDown[row][col + diag] >= diag && helperRight[row + diag][col] >= diag)
						maxBorderSize = std::max(maxBorderSize, diag + 1);
				}
			}
		}
		return maxBorderSize;
	}
};

class Rand5To7 {
	// 给定一个函数f，可以1～5的数字等概率返回一个。请加工出1～7的数字等概率返回一个的函数g。
	std::random_device rdDevice;
	std::mt19937 gen;
	std::uniform_int_distribution<int> dis;

public:
	Rand5To7(int a, int b) : gen(rdDevice()), dis(std::uniform_int_distribution<int>(a, b)) {
	}

	int f() {
		return dis(gen);
	}

	// 二进制位拼凑即可
	int gen01() {
		int res = 0;
		do {
			res = f();
		} while (res == 3);
		return res < 3 ? 0 : 1;
	}

	int g() {
		int res = 0;
		do {
			res = (gen01() << 2) + (gen01() << 1) + gen01();
		} while (res == 7);
		return res + 1;
	}

	// 进阶1
	// 给定一个函数f，可以a～b的数字等概率返回一个。请加工出c～d的数字等概率返回一个的函数g
	// 同理, 先把f加工成01发生器, 而后在进行二进制位拼凑得到0~d-c上的等概率随机数, 最后加上偏移量c返回

	// 进阶2
	// 给定一个函数f，以p概率返回0，以1-p概率返回1。请加工出等概率返回0和1的函数g
	// 这里就是01发生器需要进行一下处理, 不过也简单, 就是01和10, 如果是11和00则直接重扔, 因为01和10的概率都是p(1-p), 所以还是等概率
};

int main(int argc, char *argv[]) {
	// 绳子最多覆盖几个点
	vector<int> cords{0, 13, 24, 35, 46, 57, 60, 72, 87};
	cout << "Max Points Covered: " << CordCoverMaxPoint::cordCoverMaxPoint(cords, 6) << "\n";
	cout << "Max Points Covered: " << CordCoverMaxPoint::cordCoverMaxPointSlidingWindow(cords, 6) << "\n";

	// 最小使用的袋子
	cout << "Apple Min Bags: " << AppleMinBags::appleMinBags(20) << "\n";
	// cout << "Apple Min Bags Test: "
	//      << "\n";
	// AppleMinBags::appleMinBagsTest();
	cout << "Apple Min Bags Awesome: " << AppleMinBags::appleMinBagsAwesome(20) << "\n";

	// 吃草问题
	cout << "Eat Grass: " << EatGrass::winner(20) << "\n";
	// EatGrass::winnerTest();
	cout << "Eat Grass: " << EatGrass::winnerAwesome(20) << "\n";

	// 染色问题
	cout << "Color Left Right: " << ColorLeftRight::colorLeftRight("RGRGR") << "\n";
	cout << "Color Left Right: " << ColorLeftRight::colorLeftRight2("RGRGR") << "\n";

	// 最大正方形边长问题
	vector<vector<int>> matrix = {
	    {0, 1, 1, 1, 1},
	    {0, 1, 0, 0, 1},
	    {0, 1, 0, 0, 1},
	    {0, 1, 1, 1, 1},
	    {0, 1, 0, 1, 1}};
	cout << "Max One Birder Size: "
	     << MaxOneBorderSize::maxOneBorderSizeAwesome(matrix) << "\n";

	// 随机数范围转换问题
	Rand5To7 r(0, 5);
	cout << "Rand5To7: ";
	for (int i = 0; i < 10; i++)
		cout << r.g() << " ";
	cout << "\n";
	return 0;
}