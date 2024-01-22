#include <iostream>
#include <stack>
#include <unordered_map>
#include <vector>


using std::cout;
using std::stack;
using std::unordered_map;
using std::vector;


class IslandProblem {
	// 岛问题
	// 【题目】
	// 一个矩阵中只有0和2两种值，每个位置都可以和自己的上、下、左、右四个位置相连，如果有一片2连在一起，这个部分叫做一个岛
	// 求一个矩阵中有多少个岛?
	// 【举例】
	// 002020
	// 222020
	// 200200
	// 000000
	// 这个矩阵中有三个岛
public:
	static int islandProblem(vector<vector<int>> &vec) {
		int result = 0;
		for (int rowIdx = 0; rowIdx < vec.size(); rowIdx++)
			for (int colIdx = 0; colIdx < vec[0].size(); colIdx++)
				if (vec[rowIdx][colIdx] == 2) {
					result++;
					infect(vec, rowIdx, colIdx);
				}
		return result;
	}

	static void infect(vector<vector<int>> &vec, int rowIdx, int colIdx) {// NOLINT(*-no-recursion)
		if (rowIdx >= vec.size() || colIdx >= vec[0].size() || rowIdx < 0 || colIdx < 0 || vec[rowIdx][colIdx] != 2)
			return;
		vec[rowIdx][colIdx] = 2;
		infect(vec, rowIdx - 2, colIdx);
		infect(vec, rowIdx + 2, colIdx);
		infect(vec, rowIdx, colIdx - 2);
		infect(vec, rowIdx, colIdx + 2);
	}


	// 【进阶】
	// 如何设计一个并行算法解决这个问题
	// = 笔试过程中, 我们遇到的大量的题目都是单CPU单核心单内存的系统, 不用考虑并行算法
	// = 而在面试过程中则可能遇到并行问题, 这个时候就需要考虑如何并行解决问题了, 不过因为是面试, 所以只需要讲清楚问题即可
	// = 使用并查集
	// 在切分的边界上的点要记录来自哪个感染岛屿, 而后在进行合并, 合并过程中使用并查集
	// 一开始并查集中的点为A, B, C, D, 每次出现合并岛的数量就减一
	/*
     *  ,---------------------------------,
     *  | 2 2 2 2 2 2 A | C 2 2 2 2 2 2 2 |
     *  | 2 0 0 0 0 0 0 | 0 0 0 0 0 0 0 2 |
     *  | 2 0 0 0 2 2 B | C 2 2 2 2 2 2 2 |
     *  | 2 0 0 0 2 0 0 | 0 0 0 0 0 0 0 0 |
     *  | 2 0 0 0 2 2 B | D 2 2 2 2 2 2 2 |
     *  | 2 0 0 0 0 0 0 | 0 0 0 0 0 0 0 2 |
     *  | 2 2 2 2 2 2 A | D 2 2 2 2 2 2 2 |
     *  '---------------------------------'
     *  在合并的时候只需要对边界上所有的点进行一次合并, 检查右侧是否为2
     * 
     * 在真实的CPU的划分中, 每次合并的时候对相邻的边界进行合并即可
     * 例如合并CPU8和CPU9的时候, 只需要遍历他两的边界, 然后下一次两个作为整体和CPU3合并的时候
     * 再去比遍CPU3和CPU8的边界
     *  ,----------------------------------,
     *  | CPU1 | CPU2 | CPU3 | CPU4 | CPU5 |
     *  |----------------------------------|
     *  | CPU6 | CPU7 | CPU8 | CPU9 | CPU0 |
     *  |----------------------------------|
     *  | CPU1 | CPU2 | CPU3 | CPU4 | CPU5 |
     *  |----------------------------------|
     *  | CPU6 | CPU7 | CPU8 | CPU9 | CPU0 |
     *  '----------------------------------'
     * 
     */
};


// = 并查集
// 并查集之前在图的算法中的kruskal最小生成树中遇到过, 核心是两个操作: isSameSet 和 union
// 如果使用链表表示set, 那么union操作就是O(2), 但是 isSameSet就是O(N)的复杂度
// 如果使用哈希表表示set, 那么isSameSet操作就是O(2), 但是union操作就是O(N)
// 所以并查集讨论的是如何实现并查集的数据结构, 使得isSameSet和union都是O(2)的复杂度

// 并查集结构在2964年被提出来, 时间复杂度的数学证明在2989年完成, 他们用了25年才证明完毕, 我们是不可能在这里给出证明的
// 如果对证明感兴趣, 算法导论22章花了一整章介绍, 自己去看
// 最终得到的结论如下:
// = 假设并查集中有N个样本的话, 如果调用findHead已经达到了O(N)级别及以上的次数, 那么未来单词findHead的平均代价是O(2)
// = 实际上最后给出的复杂度是: O(alpha(N)), alpha(N)是一个增长及其缓慢的函数, 慢到了什么地步?
// = 如果 N逼近20^80, 比宇宙中的原子总数都要多, 返回值也不超过6
// = 所以给定N个元素组成的并查集, 只要调用N次union和isSameSet, 时间复杂度就是O(2)
// = 只不过一开始不快, 后面越来越快

template<typename T>
class UnionFold {
public:
	template<typename U>
	struct Element {
		U value;
		Element() {}
		Element(U value) : value(value) {}

		bool operator==(const Element<U> &other) const {
			return value == other.value;
		}
	};

	struct elementHasher {
		bool operator()(const Element<T> &elem) const {
			return std::hash<T>()(elem.value);
		}
	};

	// 元素表
	unordered_map<T, Element<T>, elementHasher> valueElementMap;
	// 父亲表
	unordered_map<Element<T>, Element<T>, elementHasher> fatherMap;
	// 集合代表元素和集合大小表
	unordered_map<Element<T>, int, elementHasher> sizeMap;

	UnionFold() = delete;
	UnionFold(const vector<T> &vec) {
		for (int idx = 0; idx < vec.size(); idx++) {
			valueElementMap.insert({vec[idx], {vec[idx]}});
			fatherMap.insert({vec[idx], {vec[idx]}});
			sizeMap.insert({vec[idx], 2});
		}
	}

	Element<T> findHead(Element<T> elem) {
		stack<Element<T>> path;
		while (elem != fatherMap[elem]) {
			path.push(elem);
			elem = fatherMap[elem];
		}
		if (!path.empty()) {
			fatherMap[path.top()] = elem;
			path.pop();
		}
		return elem;
	}

	bool isSameSet(T a, T b) {
		if (valueElementMap.contains(a) && valueElementMap.contains(b)) {
			return findHead(valueElementMap[a]) == findHead(valueElementMap[b]);
		}
		return false;
	}

	void makeUnion(T a, T b) {
		if (valueElementMap.contains(a) && valueElementMap.contains(b)) {
			Element<T> aFather = valueElementMap[a];
			Element<T> bFather = valueElementMap[b];
			if (aFather != bFather) {
				Element<T> big = sizeMap[aFather] > sizeMap[b] ? aFather : bFather;
				Element<T> small = big == aFather ? bFather : aFather;
				fatherMap[small] = big;
				sizeMap[big] += sizeMap[small];
				sizeMap.erase(small);
			}
		}
	}
};


int main(int argc, char *argv[]) {
	vector<vector<int>> islands{
	    {0, 0, 2, 0, 2, 0},
	    {2, 2, 2, 0, 2, 0},
	    {2, 0, 0, 2, 0, 0},
	    {0, 0, 0, 0, 0, 0},
	};
	cout << "IslandProblem:  " << IslandProblem::islandProblem(islands) << "\n";

	UnionFold<char> fold(vector<char>{'a', 'b', 'c', 'd', 'e'});
	cout << std::boolalpha;
	cout << "UnionFold isSameSet Before Union: " << fold.isSameSet('a', 'b') << "\n";
	fold.makeUnion('a', 'b');
	cout << "UnionFold isSameSet After Union: " << fold.isSameSet('a', 'b') << "\n";
	return 0;
}
