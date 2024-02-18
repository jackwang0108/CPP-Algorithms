#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::string;
using std::vector;

template<typename T>
std::ostream &operator<<(std::ostream &os, const vector<T> &vec) {
	os << "{";
	for (int i = 0; i < vec.size(); i++)
		cout << i << (i == vec.size() - 1 ? "}" : ", ");
	return os;
}

int main(int argc, char *argv[]) {
	return 0;
}