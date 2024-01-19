#include <map>
#include <set>
#include <vector>
#include <utility>
#include <iomanip>
#include <iostream>

using std::cout;
using std::map;
using std::pair;
using std::set;
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

struct Node
{
    int in = 0;
    int out = 0;
    int value = 0;
    vector<Node *> nexts;
    vector<Edge *> edges;

    // POD会随机初始化, 非POD则是0初始化
    explicit Node(int value) : value(value) {}
};

struct Edge
{
    int weight = 0;
    Node *from;
    Node *to;

    explicit Edge(int weight) : weight(weight) {}
    Edge(int weight, Node *from, Node *to) : weight(weight), from(from), to(to) {}
};

class Graph
{
public:
    set<Edge *> edges;
    map<const int, Node *> nodes;

    Graph() {}
    explicit Graph(vector<vector<int>> &&matrix)
    {
        int **intMatrix = new int *[matrix.size()];
        for (int edgeIdx = 0; edgeIdx < matrix.size(); edgeIdx++)
            intMatrix[edgeIdx] = new int[matrix[edgeIdx].size()];

        for (int edgeIdx = 0; edgeIdx < matrix.size(); edgeIdx++)
            for (int i = 0; i < matrix.size(); i++)
                intMatrix[edgeIdx][i] = matrix[edgeIdx][i];

        this->~Graph();
        new (this) Graph(intMatrix, matrix.size());

        for (int edgeIdx = 0; edgeIdx < matrix.size(); edgeIdx++)
            delete[] intMatrix[edgeIdx];
        delete[] intMatrix;
    }

    // 边表示法
    explicit Graph(int **matrix, int n)
    {
        for (int i = 0; i < n; i++)
        {
            int fromNodeIdx = matrix[i][1];
            int toNodeIdx = matrix[i][2];
            int weightValue = matrix[i][0];
            // 加入两个点
            if (!nodes.contains(fromNodeIdx))
                nodes.insert(std::make_pair(fromNodeIdx, new Node(fromNodeIdx)));
            if (!nodes.contains(toNodeIdx))
                nodes.insert(std::make_pair(toNodeIdx, new Node(toNodeIdx)));
            // 加入边
            Node *toNode = nodes[toNodeIdx];
            Node *fromNode = nodes[fromNodeIdx];
            Edge *newEdge = new Edge(weightValue, fromNode, toNode);
            edges.insert(newEdge);

            // 同步两个点的状态
            fromNode->nexts.push_back(toNode);
            fromNode->out++;
            toNode->in++;
            fromNode->edges.push_back(newEdge);
        }
    }

    friend std::ostream &operator<<(std::ostream &os, Graph &g)
    {
        os << "Graph at " << std::showbase << std::hex << &g << "\n"
           << std::dec;

        for (auto edgeIter = g.edges.begin(); edgeIter != g.edges.end(); edgeIter++)
            os << "Edge [weight=" << std::setw(2) << (*edgeIter)->weight << "], "
               << std::setw(2) << (*edgeIter)->from->value << " --> "
               << std::setw(2) << (*edgeIter)->to->value
               << "\n";
        return os;
    }
};

int main(int argc, char *argv[])
{
    Graph g(vector<vector<int>>{{5, 0, 1}, {3, 1, 2}, {7, 0, 2}});
    cout << g;
    return 0;
}