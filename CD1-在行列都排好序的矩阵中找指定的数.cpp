#include <iostream>
#include <vector>
using namespace std;

// = 描述
// 给定一个N×M的整形矩阵matrix和一个整数K, matrix的每一行和每一列都是排好序的。 实现一个函数，判断K是否在matrix中
// [要求]
// 时间复杂度为O(N+M)，额外空间复杂度为O(1)。
// = 输入描述：
// 第一行有三个整数N, M, K
// 接下来N行，每行M个整数为输入的矩阵
// = 输出描述：
// 若K存在于矩阵中输出"Yes"，否则输出"No"
// = 示例1
// 输入: 2 4 5
//      1 2 3 4
//      2 4 5 6
// 输出: Yes
// = 示例2
// 输入: 2 4 233
//      1 2 3 4
//      2 4 5 6
// 输出: No

int main() {
	int rowNum, colNum, numToFind;
	cin >> rowNum >> colNum >> numToFind;
	vector<vector<int>> nums;
	int temp;
	for (int i = 0; i < rowNum; i++) {
		nums.emplace_back();
		for (int j = 0; j < colNum; j++) {
			cin >> temp;
			nums[i].push_back(temp);
		}
	}

	for (int rowIdx = 0; rowIdx < rowNum; rowIdx++) {
		if (nums[rowIdx][colNum - 1] == numToFind) {
			cout << "Yes";
			return 0;
		}
		if (nums[rowIdx][colNum - 1] < numToFind)
			continue;
		for (int colIdx = 0; colIdx < colNum; colIdx++)
			if (nums[rowIdx][colIdx] == numToFind) {
				cout << "Yes";
				return 0;
			}
	}
	cout << "No";
	return 0;
}
// 64 位输出请用 printf("%lld")