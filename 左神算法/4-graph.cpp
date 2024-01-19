#include <map>
#include <set>
#include <stack>
#include <queue>
#include <vector>
#include <utility>
#include <iomanip>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <iostream>

using std::cout;
using std::map;
using std::pair;
using std::priority_queue;
using std::queue;
using std::set;
using std::shared_ptr;
using std::stack;
using std::unordered_map;
using std::unordered_set;
using std::vector;

// 图的两种表示方法:

// 1)邻接表法
//      A: [C, D]
//      B: [C]
//      C: [A, B, D]
//      D: [A, C]

// 2)邻接矩阵法 可以表示带权图
//      \   A   B   C   D
//      A   0   N   7   3
//      B   N   0   2   N
//      C   7   2   0   5
//      D   3   N   5   0

// 上面两种表示法可以表示任意的图, 有时候一些特殊的图可以直接用数组表示, 例如:
// vec = [5, 2, 2, 4, 2, 1]
// vec[i]表示第i个城市有一条路通往第vec[i]个城市
// 这种表示方法只能表示只有一条出边的图

// 图之所以麻烦就是麻烦在面对不同的特殊的图可以由不同的特殊的表示方式, 那么就需要针对不同的表示的数据结构
// 重新实现一遍我们已经掌握的算法. 目前笔试面试为了简单考的比较多的就是数组表示图. 图一般不太考, 一般面试三家公司会有一家考

// 如何准备? 用自己最喜欢的图的表示方法把图的算法全都实现一遍, 然后考试的时候在外写一个转换.
// 例如下面是左神推荐的一种表示方式(经过大量的题总结出来的比较好的)

struct Node;
struct Edge;

struct Node {
	int in = 0;
	int out = 0;
	int value = 0;
	vector<Node *> nexts{};
	vector<Edge *> edges{};

	// POD会随机初始化, 非POD则是0初始化
	explicit Node(int value) : value(value), nexts({}), edges({}) {}
};

struct Edge {
	int weight = 0;
	Node *from = nullptr;
	Node *to = nullptr;

	explicit Edge(int weight) : weight(weight), from(nullptr), to(nullptr) {}

	Edge(int weight, Node *from, Node *to) : weight(weight), from(from), to(to) {}

	bool operator<(const Edge &other) const {
		return weight > other.weight;
	}
};

std::ostream &operator<<(std::ostream &os, const Edge &edge) {
	os << std::setw(2) << edge.from->value << " --> "
	   << std::setw(2) << edge.to->value;
	return os;
}

class Graph {
public:
	set<Edge *> edges;
	map<int, Node *> IndexNodeMap;

	Graph() = default;

	explicit Graph(vector<vector<int>> &&matrix) : edges({}), IndexNodeMap({}) {
		int **intMatrix = new int *[matrix.size()];
		for (int edgeIdx = 0; edgeIdx < matrix.size(); edgeIdx++)
			intMatrix[edgeIdx] = new int[3];

		for (int edgeIdx = 0; edgeIdx < matrix.size(); edgeIdx++)
			for (int i = 0; i < 3; i++)
				intMatrix[edgeIdx][i] = matrix[edgeIdx][i];

		this->~Graph();
		new(this) Graph(intMatrix, matrix.size());

		for (int edgeIdx = 0; edgeIdx < matrix.size(); edgeIdx++)
			delete[] intMatrix[edgeIdx];
		delete[] intMatrix;
	}

	// 边表示法
	Graph(int **matrix, size_t n) : edges({}), IndexNodeMap({}) {
		for (int i = 0; i < n; i++) {
			int fromNodeIdx = matrix[i][1];
			int toNodeIdx = matrix[i][2];
			int weightValue = matrix[i][0];
			// 加入两个点
			if (!IndexNodeMap.contains(fromNodeIdx))
				IndexNodeMap.insert(std::make_pair(fromNodeIdx, new Node(fromNodeIdx)));
			if (!IndexNodeMap.contains(toNodeIdx))
				IndexNodeMap.insert(std::make_pair(toNodeIdx, new Node(toNodeIdx)));
			// 加入边
			Node *toNode = IndexNodeMap[toNodeIdx];
			Node *fromNode = IndexNodeMap[fromNodeIdx];
			Edge *newEdge = new Edge(weightValue, fromNode, toNode);
			edges.insert(newEdge);

			// 同步两个点的状态
			fromNode->nexts.push_back(toNode);
			fromNode->out++;
			toNode->in++;
			fromNode->edges.push_back(newEdge);
		}
	}

	friend std::ostream &operator<<(std::ostream &os, Graph &g) {
		os << "Graph at " << std::showbase << std::hex << &g << "\n"
		   << std::dec;

		for (auto edge: g.edges)
			os << "Edge [weight=" << std::setw(2) << edge->weight << "], " << *edge << "\n";
		return os;
	}
};

template<typename T>
std::ostream &operator<<(std::ostream &os, vector<T> &&vec) {
	for (auto &i: vec)
		cout << i << " ";
	return os;
}

std::ostream &operator<<(std::ostream &os, vector<Node *> &&vec) {
	for (auto &i: vec)
		cout << i->value << " ";
	return os;
}

class GraphTraversal {
public:
	// 图的宽度优先遍历和树不同, 因为可能会有环, 因此需要额外的一个hashSet来判断这个节点是否已经进过队列
	static std::ostream &boardFirstSearch(Node *node) {
		if (node == nullptr)
			return std::cout;
		set<Node *> queuedNodes;
		queue<Node *> unvisitedNodes;

		queuedNodes.insert(node);
		unvisitedNodes.push(node);
		while (!unvisitedNodes.empty()) {
			Node *currNode = unvisitedNodes.front();
			unvisitedNodes.pop();
			cout << currNode->value << " ";
			for (auto &nextNode: currNode->nexts)
				if (!queuedNodes.contains(nextNode))
					unvisitedNodes.push(nextNode), queuedNodes.insert(nextNode);
		}
		return std::cout;
	}

	static std::ostream &depthFirstSearch(Node *node) {
		if (node == nullptr)
			return std::cout;
		set<Node *> stackedNodes;
		// 深度优先的栈中最后会保存每一次包含所有节点的路
		stack<Node *> unvisitedNodes;

		stackedNodes.insert(node);
		unvisitedNodes.push(node);
		// = 深度优先处理是在某一个节点进入set的时候处理的
		cout << node->value << " ";
		while (!unvisitedNodes.empty()) {
			Node *currNode = unvisitedNodes.top();
			unvisitedNodes.pop();
			for (auto &nextNode: currNode->nexts) {
				if (!stackedNodes.contains(nextNode)) {
					unvisitedNodes.push(currNode);
					unvisitedNodes.push(nextNode);
					stackedNodes.insert(nextNode);
					cout << nextNode->value << " ";
					// 不在看其他的孩子, 先看当前孩子的孩子
					break;
				}
			}
		}
		return cout;
	}
};

class GraphProblems {
public:
	// 拓扑排序
	static vector<Node *> SortedTopology(Graph &graph) {
		// 节点-入度表
		map<Node *, int> inMap;
		// 入度为0的结点表
		queue<Node *> zeroInQueue;
		// 遍历结点, 找到所有的0入度的结点
		for (auto &indexNodePair: graph.IndexNodeMap) {
			inMap.insert(std::make_pair(indexNodePair.second, indexNodePair.second->in));
			if (indexNodePair.second->in == 0)
				zeroInQueue.push(indexNodePair.second);
		}

		vector<Node *> result;
		while (!zeroInQueue.empty()) {
			Node *currNode = zeroInQueue.front();
			zeroInQueue.pop();
			result.push_back(currNode);
			// 将Node连接的边的入度全部减1
			for (auto &node: currNode->nexts)
				if (--inMap[node] == 0)
					zeroInQueue.push(node);
		}

		return result;
	}

	class KruskalNodeSets {
	private:
		map<Node *, vector<Node *>> nodeSetMap;

	public:
		KruskalNodeSets() = delete;

		explicit KruskalNodeSets(vector<Node *> &vec) {
			for (Node *node: vec) {
				vector<Node *> v{node};
				nodeSetMap.insert(std::make_pair(node, v));
			}
		}

		bool isSameSet(Node *node1, Node *node2) {
			return nodeSetMap[node1] == nodeSetMap[node2];
		}

		void makeUnion(Node *node1, Node *node2) {
			vector<Node *> node1Set = nodeSetMap[node1];
			vector<Node *> node2Set = nodeSetMap[node2];
			for (Node *node: node2Set) {
				nodeSetMap.erase(node);
				nodeSetMap.insert(std::make_pair(node, node1Set));
				// nodeSetMap[node] = node1SetPtr;
				node1Set.push_back(node);
			}
		}
	};

	// 图的最小生成树-Kruskal算法, 从边考虑, 如果每次添加的边不成环, 那么就加入
	static vector<Edge> kruskalMinimumSpanningTree(Graph &graph) {
		vector<Node *> nodes;
		nodes.reserve(graph.IndexNodeMap.size());
		for (auto [idx, node]: graph.IndexNodeMap)
			nodes.push_back(node);

		// 排查是否有环
		KruskalNodeSets kSets(nodes);

		priority_queue<Edge> edgeQueue;
		for (Edge *edge: graph.edges)
			edgeQueue.push(*edge);

		vector<Edge> result;
		while (!edgeQueue.empty()) {
			Edge edge = edgeQueue.top();
			edgeQueue.pop();
			if (!kSets.isSameSet(edge.from, edge.to)) {
				result.push_back(edge);
				kSets.makeUnion(edge.from, edge.to);
			}
		}

		return result;
	}

	static vector<Edge> primMinimumSpanningTree(Graph &graph) {
		vector<Edge> result;
		priority_queue<Edge> selectableEdgeQueue;
		unordered_set<Node *> selectedNodeSet;

		vector<Node *> allNodes;
		for (auto &indexNodePair: graph.IndexNodeMap)
			allNodes.push_back(indexNodePair.second);

		// 处理深林, 即一个图有三个互相不连通的子图
		for (Node *node: allNodes) {
			if (!selectedNodeSet.contains(node)) {
				selectedNodeSet.insert(node);
				// 解锁当前点相连的所有的边
				for (Edge *connectedEdge: node->edges)
					selectableEdgeQueue.push(*connectedEdge);

				// 连接其他的点
				while (!selectableEdgeQueue.empty()) {
					Edge nextEdge = selectableEdgeQueue.top();
					selectableEdgeQueue.pop();
					Node *nextNode = nextEdge.to;
					if (!selectedNodeSet.contains(nextNode)) {
						result.push_back(nextEdge);
						selectedNodeSet.insert(nextNode);
						for (Edge *connectedEdge: nextNode->edges)
							selectableEdgeQueue.push(*connectedEdge);
					}
				}
			}
		}
		return result;
	}

	static unordered_map<Node *, int> dijkstar(Graph &graph, Node *node) {
		unordered_map<Node *, int> distanceMap;
		distanceMap.insert(std::make_pair(node, 0));
		unordered_set<Node *> selectedNodes;
		Node *minNode = getUnselectedNodeMinDistance(distanceMap, selectedNodes);

		while (minNode != nullptr) {
			int distance = distanceMap[minNode];
			for (Edge *edge: minNode->edges) {
				Node *toNode = edge->to;
				if (!distanceMap.contains(toNode))
					distanceMap.insert({toNode, distance + edge->weight});
				distanceMap[toNode] = std::min(distanceMap[toNode], distance + edge->weight);
			}
			selectedNodes.insert(minNode);
			minNode = getUnselectedNodeMinDistance(distanceMap, selectedNodes);
		}
		return distanceMap;
	}

	static Node *
	getUnselectedNodeMinDistance(unordered_map<Node *, int> &distanceMap, unordered_set<Node *> &selectedNodes) {
		std::pair<Node *, int> minPair{nullptr, INT16_MAX};
		for (auto &pair: distanceMap)
			if (!selectedNodes.contains(pair.first) && minPair.second > pair.second)
				minPair = pair;
		return minPair.first;
	}
};

int main(int argc, char *argv[]) {
	Graph g(vector<vector<int>>{
		{1, 0, 1},
		{1, 0, 2},
		{1, 0, 3},
		{1, 1, 2},
		{1, 1, 4},
		{1, 2, 3},
		{1, 2, 4},
		{1, 3, 4}});
	cout << g;
	// 广度优先遍历
	cout << "Board First Search: ", GraphTraversal::boardFirstSearch(g.IndexNodeMap[0]) << "\n";
	// 深度优先遍历
	cout << "Depth First Search: ", GraphTraversal::depthFirstSearch(g.IndexNodeMap[0]) << "\n";

	// 拓扑排序
	Graph TopologyGraph(vector<vector<int>>{
		{1, 3, 2},
		{1, 3, 1},
		{1, 2, 1},
		{1, 2, 0},
		{1, 1, 0}});
	auto result = GraphProblems::SortedTopology(TopologyGraph);
	cout << "Topology Sorted: " << GraphProblems::SortedTopology(TopologyGraph) << "\n";

	// 最小生成树
	Graph kruskalGraph(vector<vector<int>>{
		{7,     1, 2},
		{2,     1, 3},
		{100,   1, 3},
		{1000,  2, 4},
		{10000, 2, 5},
		{4,     3, 4}});
	// kruskal最小生成树算法
	vector<Edge> kruskalResult = GraphProblems::kruskalMinimumSpanningTree(kruskalGraph);
	cout << "kruskal Minimum Spanning Tree: \n";
	for (auto &edge: kruskalResult)
		cout << edge << "\n";
	cout << "\n";
	// prim最小生成树算法
	vector<Edge> primResult = GraphProblems::primMinimumSpanningTree(kruskalGraph);
	cout << "prim Minimum Spanning Tree: \n";
	for (auto &edge: primResult)
		cout << edge << "\n";
	cout << "\n";

	// dijkstar算法
	Graph dijkstarGraph(vector<vector<int>>{
		{3,  1, 2},
		{7,  1, 3},
		{10, 1, 4},
		{2,  2, 3},
		{4,  3, 4}});
	auto distanceMap = GraphProblems::dijkstar(dijkstarGraph, dijkstarGraph.IndexNodeMap[1]);
	cout << "Distance Map: "
	     << "\n";
	for (auto &i: distanceMap)
		cout << "From " << 1 << " to " << i.first->value << ", distance = " << i.second << "\n";

	return 0;
}
