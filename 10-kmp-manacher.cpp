#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::string;
using std::vector;

class KMP {
public:
	// O(M+N)
	static int kmp(const string &str, const string &subStr) {
		if (str.size() < subStr.size())
			return -1;
		// O(M)
		vector<int> next = getNext(subStr);
		int strIndex = 0, subStrIndex = 0;
		// O(N), strIndex(max(N)), strIndex-subStrIndex
		// 循环次数最多2N次
		while (strIndex < str.length() && subStrIndex < subStr.size()) {
			// strIndex+, (strIndex-subStrIndex)+
			if (str[strIndex] == subStr[subStrIndex])
				strIndex++, subStrIndex++;
			// strIndex+, (strIndex-subStrIndex)+
			else if (next[subStrIndex] == -1)
				strIndex++;
			// strIndex不变, (strIndex-subStrIndex)+
			else
				subStrIndex = next[subStrIndex];
		}

		return subStrIndex == subStr.length() ? strIndex - subStrIndex : -1;
	}

	static vector<int> getNext(const string &subStr) {
		if (subStr.length() == 1)
			return {-1};
		vector<int> next(subStr.size(), 0);
		next[0] = -1;
		int compareIndex = 0, nextFillIndex = 2;
		while (nextFillIndex < subStr.length()) {
			if (subStr[nextFillIndex] == subStr[compareIndex])
				next[nextFillIndex++] = ++compareIndex;
			else if (compareIndex > 0)
				compareIndex = next[compareIndex];
			else
				next[nextFillIndex++] = 0;
		}
		return std::move(next);
	}
};


class Manacher {
	// Manacher算法解决的问题: 字符串str中，最长回文子串的长度如何求解？
public:
	// 经典解法由于没有办法处理偶数回文子串的情况, 因此需要插入一个辅助字符来帮助. 例如:
	// 1 2 3 2 1 -> # 1 # 2 # 3 # 2 # 1 #, 此时最大回文子串长度为中间3的11, 除以2(多加了一倍的辅助字符), 得到5, 返回
	// 1 2 3 3 2 1 -> # 1 # 2 # 3 # 3 # 2 # 1 #, 此时最大会问子串长度为3中间的#, 长度为13, 除以2得到6, 返回
	// 时间复杂度是 O(N^2)
	static int classicSolution(string &str) {
		string processedStr = "#";
		for (size_t idx = 0; idx < str.length(); idx++) {
			processedStr.push_back(str[idx]);
			processedStr.push_back('#');
		}

		int result = 0;
		vector<int> primordialLength(processedStr.length(), 1);
		for (int idx = 0; idx < processedStr.length(); idx++) {
			int offset = 1;
			while (0 <= idx - offset && idx + offset < processedStr.length())
				if (processedStr[idx - offset] == processedStr[idx + offset]) {
					primordialLength[idx] = 2 * offset + 1;
					result = std::max(primordialLength[idx], result);
					offset++;
				} else
					break;
		}

		return result / 2;
	}

	// Manacher算法要求做到时间复杂度 O(N) 来解决这个问题
	// = Manacher算法中最重要的就是回文半径数组, 这个数组在很多求回文字符串题目中都会运用到
	// = 此时我们只需要去修改他的逻辑即可, 即修改Manacher算法中的几种情况即可
	static int manacher(string &str) {
		string processedStr = "#";
		for (size_t idx = 0; idx < str.length(); idx++) {
			processedStr.push_back(str[idx]);
			processedStr.push_back('#');
		}

		int max = 0;
		int centerIndex = -1;
		int rightMostIndex = -1;
		vector<int> primordialRadius(processedStr.length(), 0);
		// Manacher算法一共有两种情况:
		// 1. idx 在 rightMostIndex 外, 则暴力扩容
		// 2. idx 在 rightMostIndex 内,
		//      1) idx' 的左边界在 leftMostIndex 内, 不用扩展, 就是 idx' 对称的边界对称归来
		//      2) idx' 的左边界在 leftMostIndex 外, 不用扩展, 就是左侧边界
		//      3) idx' 的左边界恰好在 leftMostIndex 上, 还需要向外扩展, 要看下一个
		for (int idx = 0; idx < processedStr.length(); idx++) {
			// 至少不用验的区域
			// 如果 rightMostIndex > idx, 那么就是 idx 在 rightMostIndex 内, 此时取左边界和 leftMostIndex 的最小值
			// 如果 rightMostIndex < idx, 那么就是暴力扩容, 此时不需要比较自己, 所以至少为1
			primordialRadius[idx] = rightMostIndex > idx ? std::min(primordialRadius[2 * centerIndex - idx], rightMostIndex - idx) : 1;

			// 统一扩展, 由于具有数学上的保证, 所以2.1和2.2一定会失败
			while (idx + primordialRadius[idx] < processedStr.length() && idx - primordialRadius[idx] > -1) {
				if (processedStr[idx + primordialRadius[idx]] == processedStr[idx - primordialRadius[idx]])
					primordialRadius[idx]++;
				else
					break;
			}

			// 需要更新 centerIndex 和 rightMostIndex
			if (idx + primordialRadius[idx] > rightMostIndex) {
				centerIndex = idx;
				rightMostIndex = idx + primordialRadius[idx];
			}

			max = std::max(max, primordialRadius[idx]);
		}

		return max - 1;
	}
};


int main(int argc, char *argv[]) {
	string str = "abcabcababaccc";
	string subStr = "aba";
	cout << "KMP: " << KMP::kmp(str, subStr) << "\n";

	string odd = "1234321";
	string even = "1234554321";
	cout << "Classic odd: " << Manacher::classicSolution(odd) << "\n";
	cout << "Classic even: " << Manacher::classicSolution(even) << "\n";
	cout << "Manacher odd: " << Manacher::manacher(odd) << "\n";
	cout << "Manacher even: " << Manacher::manacher(even) << "\n";
	return 0;
}
