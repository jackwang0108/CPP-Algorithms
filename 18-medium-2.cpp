#include <algorithm>
#include <iostream>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using std::cout;
using std::shared_ptr;
using std::stack;
using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;

class UniqueBST {
	// 给定一个非负整数n，代表二叉树的节点个数。返回能形成多少种不同的二叉树结构
public:
	// 输入整形, 输出整形, 可以考虑打表, 不过还是先写出来正确的版本
	// 递归解法, 左树有i个节点, 右树有N-i-1个节点, i从0~N-1
	// basecase就是没有节点或者只有一个节点
	static int uniqueBST(int n) {
		// basecase
		if (n < 0)
			return 0;
		if (n == 0 || n == 1)
			return 1;
		if (n == 2)
			return 2;
		int result = 0;
		for (int leftNum = 0; leftNum <= n - 1; leftNum++) {
			int left = uniqueBST(leftNum);
			int right = uniqueBST(n - 1 - leftNum);
			result += left * right;
		}
		return result;
	}

	// 这个规律不太好观察出来, 实际上最后的结果是卡特兰数, 所以可以直接代入公式算
	static void uniqueBSTTest() {
		for (int i = 0; i < 100; i++)
			cout << i << " " << uniqueBST(i) << "\n";
	}

	// 改成动态规划, 可变参数只有一个就是n, 所以是一维的动态规划, n的变化范围是0~n
	// 所以动态规划表就是一个一维数组, 要返回的就是dp[n]
	// 然后分析basecase, 就是n=0,1是1, n=2是2, 左侧非法区域都是0
	// 然后分析填表的依赖关系, 假设现在要填f(i)位置, 那么f(i)的依赖就是依赖f(0)和f(i-1), f(1)和f(i-1-i), f(2), f(i-1-2), ...
	// 故f(i)依赖的是f(0)到f(i-1)
	// 所以填表的时候就需要有一个内层循环
	static int uniqueBSTDP(int n) {
		vector<int> dp(n + 1, 0);
		dp[0] = 1;
		for (int i = 1; i < n + 1; i++)
			// 左侧节点数是j, 右侧节点数是i-j
			for (int j = 0; j <= i - 1; j++)
				dp[i] += dp[j] * dp[i - 1 - j];
		return dp[n];
	}
};

class NeedParentheses {
	// 一个完整的括号字符串定义规则如下:
	// ①空字符串是完整的。
	// ②如果s是完整的字符串，那么(s)也是完整的。
	// ③如果s和t是完整的字符串，将它们连接起来形成的st也是完整的。
	// 例如
	// "(()())",""和"(())()"是完整的括号字符串，
	// "())(","()("和")"是不完整的括号字符串。
	// 牛牛有一个括号字符串s,现在需要在其中任意位置尽量少地添加括号,将其转化为一个完整的括号字符串。
	// 请问牛牛至少需要添加多少个括号。
public:
	// 本质上就是找有几个不完整的括号即可, 遇到左括号+1, 右括号-1
	// 如果小于0表示有没有匹配的左括号, 此时answer直接+1, 然后toBePaired变成0, 继续遍历, 最后看toBePaired剩几加几即可
	static int needParentheses(const string &str) {
		int answer = 0;
		int toBePaired = 0;
		for (int i = 0; i < str.length(); i++) {
			toBePaired += str[i] == '(' ? 1 : -1;
			if (toBePaired < 0)
				answer++, toBePaired = 0;
		}
		return answer + toBePaired;
	}
};

class SubValueEqualK {
	// 给定一个数组arr，求差值为k的去重数字对
public:
	static int subValueEqualK(const vector<int> &nums, int k) {
		// 哈希表 or 排序 + 双指针
		unordered_set<int> hashSet;
		for (int i = 0; i < nums.size(); i++)
			hashSet.insert(nums[i]);

		int ans = 0;
		for (int i = 0; i < nums.size(); i++)
			ans += hashSet.contains(nums[i] + k) ? 1 : 0;
		return ans;
	}
};

// 程序员找工作的时候遇到的题目分为两类:
//  1. 业务为主的题目
//  2. 技巧为主的题目
// 业务为主的题目是指问题的难点主要在于业务的分析, 最终的答案主要是以业务逻辑为核心
// 技巧为主的题目是指最终答案的核心在于优化, 与业务逻辑存在较大的偏差
// 两类题目在笔试和面试中的占比是:
//      业务为主        技巧为主
// 笔试    55%            45%
// 面试    30%            70%
// 因为笔试阶段需要淘汰人, 而业务为主的题目可以出的很恶心人, 让人不知道该怎么做
// 而面试阶段则需要考察人的算法基本功, 因此业务逻辑转化为算法不难, 难的是算法会不会
// 下面这个题就是业务为主的题目, 因为很难把业务和 算法结合起来

class MagicOperation {
	// 给一个包含n个整数元素的集合a，一个包含m个整数元素的集合b。
	// 定义magic操作为，从一个集合中取出一个元素，放到另一个集合里，且操作过后每个集合的平均值都大大于于操作前。
	// 注意以下两点：
	//  1）不可以把一个集合的元素取空，这样就没有平均值了
	//  2）值为x的元素从集合b取出放入集合a，但集合a中已经有值为x的元素，则a的平均值不变（因为集合元素不会重复），b的平均值可能会改变（因为x被取出了）
	// 问最多可以进行多少次magic操作？

public:
	// 平均值等于时候没法操作, 结束
	// 其余就是把平均值大的集合中大于平均值小的集合的平均值的数拿出来, 放到平均值小的集合中
	// 此外再定义一个贪心策略, 每次拿的数字都是符合要求的数的最小数, 这样保证所能够进行的操作次数最多.
	// 这就是这道题的解法
	static int magicOperation(vector<int> &nums1, vector<int> &nums2) {
		int sum1 = 0, sum2 = 0;
		int num1 = nums1.size(), num2 = nums2.size();
		for (int i = 0; i < nums1.size(); i++)
			sum1 += nums1[i];
		for (int i = 0; i < nums2.size(); i++)
			sum2 += nums2[i];
		if (average(sum1, num1) == average(sum2, num2))
			return 0;

		int greaterSum, lessSum;
		vector<int> *greaterNums, *lessNums;
		if (average(sum1, num1) > average(sum2, num2)) {
			greaterSum = sum1;
			greaterNums = &nums1;
			lessSum = sum2;
			lessNums = &nums2;
		} else {
			greaterSum = sum2;
			greaterNums = &nums2;
			lessSum = sum1;
			lessNums = &nums1;
		}

		std::sort(greaterNums->begin(), greaterNums->end());

		unordered_set<int> lessSet;
		for (auto i: *lessNums)
			lessSet.insert(i);

		int greaterSize = greaterNums->size();
		int lessSize = lessNums->size();
		int ops = 0;
		for (int i = 0; i < greaterNums->size(); i++) {
			double curr = (double) (*greaterNums)[i];
			if (curr < average(greaterSum, greaterSize) && curr > average(lessSum, lessSize) && !lessSet.contains((*greaterNums)[i])) {
				greaterSum -= curr;
				greaterSize--;
				lessSum += curr;
				lessSize++;
				lessSet.insert((*greaterNums)[i]);
				ops++;
			}
		}
		return ops;
	}

	static double average(int sum, int nums) {
		return (double) sum / (double) nums;
	}
};

class ParenthesesDepth {
	// 一个合法的括号匹配序列有以下定义:
	// ①空串""是一个合法的括号匹配序列
	// ②如果"X"和"Y"都是合法的括号匹配序列,"XY"也是一个合法的括号匹配序列
	// ③如果"X"是一个合法的括号匹配序列,那么"(X)"也是一个合法的括号匹配序列
	// ④每个合法的括号序列都可以由以上规则生成。
	// 例如:"","()","()()","((()))"都是合法的括号序列
	// 对于一个合法的括号序列我们又有以下定义它的深度:
	// ①空串""的深度是0
	// ②如果字符串"X"的深度是x,字符串"Y"的深度是y,那么字符串"XY"的深度为max(x,y)
	// ③如果"X"的深度是x,那么字符串"(X)"的深度是x+1
	// 例如:"()()()"的深度是1,"((()))"的深度是3。
	// 牛牛现在给你一个合法的括号序列,需要你计算出其深度。
public:
	static int parenthesesDepth(const string &str) {
		int maxDepth = 0, currDepth = 0;
		for (auto c: str) {
			currDepth += c == '(' ? 1 : -1;
			maxDepth = std::max(maxDepth, currDepth);
		}
		return maxDepth;
	}
};

class LongestValidParentheses {
	// 给定一个由()组成的字符串, 求出其中最长的有效子串的长度
public:
	// 字符串的题有两种, 一种是子序列, 不连续的, 一种是子串, 连续的
	// 对于子串类型的题, 往往可以针对以每个位置的字符为结尾/开头的子串求解, 然后合并起来得到最终解
	// 这个题就是, 针对每个位置的字符为结尾的子串求一下最大长度, 然后取最大值即可
	static int longestValidParentheses(const string &str) {
		vector<int> maxLength(str.length(), 0);

		int pre = 0, res = 0;
		for (int i = 0; i < str.length(); i++) {
			if (str[i] == ')') {
				pre = i - maxLength[i - 1] - 1;
				if (pre >= 0 && str[pre] == '(')
					maxLength[i] = maxLength[i - 1] + 2 + (pre - 1 > 0 ? maxLength[pre - 1] : 0);
			}
			res = std::max(res, maxLength[i]);
		}
		return res;
	}
};

template<typename T>
std::ostream &operator<<(std::ostream &os, stack<T> &st) {
	if (st.empty())
		return os;
	T curr = st.top();
	st.pop();
	os << st;
	os << curr << " ";
	st.push(curr);
	return os;
}

class StackSortStack {
	// 请编写一个程序，对一个栈里的整型数据，按升序进行排序（即排序前，栈里的数据是无序的，排序后最大元素位于栈顶）
	// 要求最多只能使用一个额外的栈存放临时数据，但不得将元素复制到别的数据结构中。
public:
	static void stackSortStack(stack<int> &st) {
		stack<int> helper;
		while (!st.empty()) {
			int curr = st.top();
			st.pop();
			while (!helper.empty() && helper.top() < curr) {
				st.push(helper.top());
				helper.pop();
			}
			helper.push(curr);
		}
		while (!helper.empty()) {
			st.push(helper.top()), helper.pop();
		}
	}
};

class NumToStringWays {
	// 将给定的数转换为字符串，原则如下：1对应a，2对应b，.....26对应z，
	// 例如12258可以转换为"abbeh","aveh","abyh","lbeh"和"lyh"，个数为5，编写一个函数，给出可以转换的不同字符串的个数。
public:
	static int numToStringWays(const string &numStr) {
		return processNumToStringWays(numStr, 0);
	}

	// 递归的解法, 就看当前这一位的可能 + 后面的可能
	static int processNumToStringWays(const string &numStr, int idx) {
		if (idx >= numStr.length())
			return 1;
		if (numStr[idx] == '0')
			return 0;
		int result = processNumToStringWays(numStr, idx + 1);
		if (numStr[idx] <= '2' && idx + 1 < numStr.length() && numStr[idx + 1] <= '6')
			result += processNumToStringWays(numStr, idx + 2);
		return result;
	}

	// 递归过程中只有一个可变变量, 所以改成动态规划就只有一个变量, 因此动态规划表就只有一个变量
	// basecase就是idx==numStr.length(), 此时就是1
	// 然后是从右往左填
	// 返回动态规划表的dp[0]
	static int processNumToStringWaysDP(const string &numStr) {
		vector<int> dp(numStr.length() + 1, 0);
		dp[numStr.length()] = 1;
		for (int i = numStr.length() - 1; i >= 0; i--) {
			dp[i] = dp[i + 1];
			if (numStr[i] <= '2' && i + 1 < numStr.length() && numStr[i + 1] <= '6')
				dp[i] += dp[i + 2];
		}
		return dp[0];
	}
};

class MaxSumInTree {
	// 二叉树每个结点都有一个int型权值，给定一棵二叉树，要求计算出从根结点到叶结点的所有路径中，权值和最大的值为多少。
public:
	struct TreeNode {
		int weight = 0;
		shared_ptr<TreeNode> left;
		shared_ptr<TreeNode> right;

		TreeNode() {}
		explicit TreeNode(int weight) : weight(weight) {}
		explicit TreeNode(const vector<int> &weights) {
			if (weights.empty())
				return;
			weight = weights[0];
			left = buildTree(weights, 1);
			right = buildTree(weights, 2);
		}

		shared_ptr<TreeNode> buildTree(const vector<int> &weights, int idx) {
			if (idx >= weights.size() || weights[idx] == -1)
				return nullptr;
			shared_ptr<TreeNode> root = std::make_shared<TreeNode>(weights[idx]);
			root->left = buildTree(weights, idx * 2 + 1);
			root->right = buildTree(weights, idx * 2 + 2);
			return std::move(root);
		}
	};

	static int maxSumInTree(const shared_ptr<TreeNode> root) {
		return processMaxSumInTree(root);
	}

	static int processMaxSumInTree(const shared_ptr<TreeNode> root) {
		if (root == nullptr)
			return 0;
		return root->weight + std::max(processMaxSumInTree(root->left), processMaxSumInTree(root->right));
	}
};

class FindNumInSortedMatrix {
	// 给定一个元素为非负整数的二维数组matrix，每行和每列都是从小到大有序的。
	// 再给定一个非负整数aim，请判断aim是否在matrix中, 要求时间复杂度O(M+N)
public:
	static bool findNumInSortedMatrix(vector<vector<int>> &matrix, int aim) {
		int currRow = 0, currCol = matrix[0].size() - 1;
		while (0 <= currRow && currRow < matrix.size() && 0 <= currCol && currCol < matrix[0].size()) {
			if (matrix[currRow][currCol] == aim)
				return true;
			else if (matrix[currRow][currCol] > aim)
				currCol--;
			else if (matrix[currRow][currCol] < aim)
				currRow++;
		}
		return false;
	}
};

int main(int argc, char *argv[]) {
	// 二叉树结构问题
	cout << "Unique BST: " << UniqueBST::uniqueBST(3) << "\n";
	cout << "Unique BST DP: " << UniqueBST::uniqueBSTDP(3) << "\n";

	// 完整括号字符串问题
	cout << "needParentheses: " << NeedParentheses::needParentheses("())(") << "\n";
	cout << "needParentheses: " << NeedParentheses::needParentheses("(()())") << "\n";

	// 差值为k的去重数字对
	cout << "SubValueEqualK: " << SubValueEqualK::subValueEqualK({3, 2, 5, 7, 0, 0}, 2) << "\n";

	// magic操作数
	vector<int> v1{1, 2, 5};
	vector<int> v2{2, 3, 4, 5, 6};
	cout << "Max Magic Operation: " << MagicOperation::magicOperation(v1, v2) << "\n";

	// 最大深度
	cout << "Max Depth: " << ParenthesesDepth::parenthesesDepth("((()))") << "\n";

	// 最长合法子串
	cout << "Longest Valid Parentheses: " << LongestValidParentheses::longestValidParentheses("(()())(())") << "\n";

	// 排序栈
	stack<int> st;
	for (int i: {9, 3, 1, 4, 5, 2, 6, 7})
		st.push(i);
	cout << "{ " << st << "}"
	     << "\n";
	StackSortStack::stackSortStack(st);
	cout << "{ " << st << "}"
	     << "\n";


	// 字符串转换
	cout << "Num To Strings: " << NumToStringWays::numToStringWays("12258") << "\n";
	cout << "Num To Strings: " << NumToStringWays::processNumToStringWaysDP("12258") << "\n";

	// 二叉树最大权值
	shared_ptr<MaxSumInTree::TreeNode> tree = std::make_shared<MaxSumInTree::TreeNode>(vector<int>{1, 2, 3, 4, 5, 6, 7});
	cout << "Max Sum In Tree: " << MaxSumInTree::maxSumInTree(tree) << "\n";

	// 寻找数字
	vector<vector<int>> matrix{
	    {1, 5, 9, 10},
	    {2, 6, 11, 13},
	    {7, 9, 15, 17}};
	cout << "Find Num In Sorted Matrix: " << std::boolalpha << FindNumInSortedMatrix::findNumInSortedMatrix(matrix, 12) << "\n";
	return 0;
}