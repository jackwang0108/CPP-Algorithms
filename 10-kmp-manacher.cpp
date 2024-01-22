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


int main(int argc, char *argv[]) {
	string str = "abcabcababaccc";
	string subStr = "aba";
	cout << "KMP: " << KMP::kmp(str, subStr) << "\n";
	return 0;
}