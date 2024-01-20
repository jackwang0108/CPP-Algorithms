#include <algorithm>
#include <functional>
#include <iostream>
#include <queue>
#include <vector>

using std::cout;
using std::priority_queue;
using std::vector;

// 贪心算法的在笔试时的解题套路
// 1，实现一个不依靠贪心策略的解法X，可以用最暴力的尝试
// 2，脑补出贪心策略A、贪心策略B、贪心策略C...
// 3，用解法X和对数器，去验证每一个贪心策略，用实验的方式得知哪个贪心策略正确
// 4，不要去纠结贪心策略的证明, 每一个问题具体地证明可能千奇百怪

// 贪心策略在实现时，经常使用到的技巧：
// 1，根据某标准建立一个比较器来排序
// 2，根据某标准建立一个比较器来组成堆

// = 针对贪心, 只需要把暴力的解法写熟练, 然后把对数器写熟练即可, 贪心策略的题目最后贪心的解法一定都是非常简单的

// 注意, 贪心策略会涉及到自定义的比较操作, 这个时候就要求自定义的比较操作是有效的, 即具备传递性
// 例如: 给定一个字符串数组, 要求返回具有最小字典序的拼接结果
// 例: 给定 S=["ba", "b"], 返回 R="bab", 因为可能的拼接结果为: "bab" 和 "bba", 而 "bab" 字典序小于 "bba", 因此返回 "bba"
// 那么我们可以定义如下的贪心策略: 字符串x和字符串y, 如果 x+y <= y+x的话, 那么拼接的时候就把x放到y的前面, 所有的字符串从小到大排序后拼接起来返回
// 得到的就是结果
// = 这个时候就要求这种比较具有传递性, 即若 a+b <= b+a, b+c <= c+b, 要求 a+c <= c+a成立
// 这里的证明可以采用把字符串视为k进制数的方式来在数学上描述字符串拼接
// 设m(b)表示k^b的长度, 则a+b = a*m(b)+b, 故有
//              a*m(b) + b <= b*m(a) + a   和    b*m(c) + c <= c*m(b) + b
// 成立, 故有
//              a*m(b)*c <= b*m(a)*c + a*c - b*c    和 b*m(c)*a + c*a - b*a <= c*m(b)*a
// 成立, 故有
//              b*m(c)*a + c*a - b*a <= b*m(a)*c + a*c - b*c
// 成立, 化简得
//              m(c)*a + c <= m(a)*c + a
// 即:
//              a+c <= c+a
// 成立
// 到这里只是证明了这种比较具有传递性, 接下来还要证明这种贪心算法是正确的
// 那么就先假设排序后得到的序列为: [..., ..., ..., ..., ...]
// 假设现在任意交换两个字符串, 然后证明交换之后的结果比原来的结果大.
//      情况一: 交换的两个字符串相邻, 即 [..., ..., a, b, ..., ...], 则交换后为 [..., ..., b, a, ..., ...]
//             由于贪心策略知: a+b <= b+a, 所以此时交换后比原来的结果要大
//      情况二: 交换的两个字符串不相邻, 假设中间有m个字符串(这里以两个为例), 即 [..., ..., a, m1, m2, b, ..., ...],
//             则交换后为 [..., ..., b, m1, m2, a, ..., ...]
//             则 [..., ..., a, m1, m2, b, ..., ...] <= [..., ..., m1, a, m2, b, ..., ...]
//                <= [..., ..., m1, m2, ma, b, ..., ...] <= [..., ..., m1, m2, b, a, ..., ...]
//                <= [..., ..., m1, b, m2, a, ..., ...] <= [..., ..., b, m1, m2, a, ..., ...]
//             故原来的是最小的
// 然后还需要证明, 任意交换三个, 交换四个 字符串得到的都比原来的大, 这就需要数学归纳法
// = 所以贪心算法在做题的时候, 只需要策略中的比较具有传递性即可, 剩下的就是考对数器去验证就好了, 打ACM每年最后final的队伍
// = 在赛场上都是这样的, 贪心的题四个人在一起想七八个策略, 然后对数器比较下看看对不对, 如果ok的话就直接交上去了, 然后AC
// = 很多都是比完赛了之后都不知道为什么对, 回来了之后才数学推理证明
// = 只不过是他们平时练习的时候贪心的题目做的足够的多, 直觉非常敏锐, 所以能很快做出来


template<typename T>
std::ostream &operator<<(std::ostream &os, vector<T> &vec) {
	for (auto &v: vec)
		cout << v << "\n";
	return os;
}


class BestMeetingArrangement {
	// 一些项目要占用一个会议室宣讲，会议室不能同时容纳两个项目的宣讲。
	// 给你每一个项目开始的时间和结束的时间(给你一个数组，里面是一个个具体的项目)，你来安排宣讲的日程，要求会议室进行的宣讲的场次最多。
	// 返回这个最多的宣讲场次。

	// 贪心策略: 每次安排的会议使得剩余的时间最长
public:
	struct Meeting {
		int startTime;
		int endTime;

		Meeting() : startTime(0), endTime(0) {}

		Meeting(int start, int end) : startTime(start), endTime(end) {}

		bool operator<(Meeting &other) const {
			return endTime < other.endTime;
		}

		friend std::ostream &operator<<(std::ostream &os, Meeting &m) {
			os << "Start: " << m.startTime << ", End: " << m.endTime;
			return os;
		}
	};

	static vector<Meeting> bestMeetingArrangement(vector<Meeting> &meetings, int startTimePoint) {
		std::function<bool(Meeting &, Meeting &)> compareEndTime = [](Meeting &m1, Meeting &m2) -> bool {
			return m1.endTime < m2.endTime;
		};
		std::sort(meetings.begin(), meetings.end(), compareEndTime);

		vector<Meeting> result;
		for (auto &meeting: meetings) {
			if (startTimePoint <= meeting.startTime) {
				result.push_back(meeting);
				startTimePoint = meeting.endTime;
			}
		}
		return result;
	}
};

class SplitGold {
	// 一块金条切成两半，是需要花费和长度数值一样的铜板的。比如长度为20的金条，不管切成长度多大的两半，都要花费20个铜板。
	// 一群人想整分整块金条，怎么分最省铜板? 输入一个数组，表示每个人想要的金条的总长度, 返回分割的最小代价
	// 例如, 给定数组{10,20,30}，代表一共三个人，整块金条长度为10+20+30=60。
	// 金条要分成10,20,30三个部分。如果先把长度60的金条分成10和50，花费60；再把长度50的金条分成20和30，花费50；一共花费110个铜板。
	// 但是如果先把长度60的金条分成30和30，花费60；再把长度30的金条分成10和20，花费30；一共花费90的铜板。

public:
	// 贪心策略: 建立一个小根堆, 每次pop出两个数, 结合之后放回小根堆, 一直直到最后只有一个数, 此时就是最小的代价
	// 其次略就是每次划分的时候尽可能划分出来最大的数的结合
	static int lessMoneySplitGold(vector<int> &vec) {
		priority_queue<int, vector<int>, std::greater<>> minHeap;
		for (auto i: vec)
			minHeap.push(i);

		int result = 0, curr = 0;
		while (minHeap.size() > 1) {
			int first = minHeap.top();
			minHeap.pop();
			int second = minHeap.top();
			minHeap.pop();
			curr = first + second;
			result += curr;
			minHeap.push(curr);
		}
		return result;
	}
};


class MaxIPOProfit {
	// 给定正数数组costs, 正数数组profits, 正数k, 正数m
	// 含义分别是: costs[i]表示i号项目的花费, profits[i]表示i号项目在扣除花费之后还能挣到的钱(利润)
	//           k表示你只能最多做k个项目, m表示你初始的资金
	// 说明：你每做完一个项目，马上获得的收益，可以支持你去做下一个项目。
	// 输出：你最后获得的最大钱数。

	// 贪心策略: 每次做的项目是当前能做的项目中收益最大的
public:
	struct IPOProject {
		int cost = 0;
		int profit = 0;
	};

	static int maxIPOProfit(vector<int> costs, vector<int> profits, int k, int m) {

		std::function<bool(IPOProject &, IPOProject &)> costComparator = [](IPOProject &p1, IPOProject &p2) -> bool {
			return p1.cost > p2.cost;
		};

		std::function<bool(IPOProject &, IPOProject &)> earnComparator = [](IPOProject &p1, IPOProject &p2) -> bool {
			return p1.cost + p1.profit < p2.cost + p2.profit;
		};

		// 锁住的项目
		priority_queue<IPOProject, vector<IPOProject>, decltype(costComparator)> costQueue(costComparator);
		// 已解锁的项目
		priority_queue<IPOProject, vector<IPOProject>, decltype(earnComparator)> earnQueue(earnComparator);

		for (int i = 0; i < costs.size(); i++)
			costQueue.push({costs[i], profits[i]});

		// 进行k轮
		for (int i = 0; i <= k; i++) {
			// 所有能做的项目
			while (!costQueue.empty() && costQueue.top().cost <= m) {
				earnQueue.push(costQueue.top()), costQueue.pop();
			}
			// 如果没有能做的项目
			if (earnQueue.empty())
				return m;
			m += earnQueue.top().profit, earnQueue.pop();
		}
		return m;
	}
};


int main(int argc, char *argv[]) {
	vector<BestMeetingArrangement::Meeting> meetings{{1, 2},
	                                                 {2, 3},
	                                                 {3, 4}};
	cout << meetings;

	vector<int> golds{10, 20, 30};
	cout << "less money: " << SplitGold::lessMoneySplitGold(golds) << "\n";

	vector<int> costs{3, 1, 4, 9, 9};
	vector<int> profit{1, 2, 3, 7, 4};
	cout << "Max IPO Profit: " << MaxIPOProfit::maxIPOProfit(costs, profit, 4, 1) << "\n";

	return 0;
}
