#include <iostream>
#include <vector>

using std::cout;
using std::vector;


// 位运算的题目都是神仙题目, 也就是说如果你见过, 那你就会, 你没遇到过, 那你就不会


class BitOperation {
public:
	// 给定两个有符号32位整数a和b，返回a和b中较大的。
	// 【要求】
	// 不要做任何比较判断。
	// = 这个方法最核心的就是把运用互斥变量, 但是没有考虑到溢出的问题
	static int bitCompareMax1(int a, int b) {
		int c = a - b;
		int sign = getSign(c);
		int signFlipped = flipBit(sign);
		// 把返回最大值转化为互斥条件相加
		return a * sign + b * signFlipped;
	}

	// = 考虑到溢出的做法
	static int bitCompareMax2(int a, int b) {
		int c = a - b;
		int signA = getSign(a);
		int signB = getSign(b);
		int signC = getSign(c);
		// AB同号则不可能溢出, 只有两个异号才有肯呢个溢出
		int diffSign = signA ^ signB;
		int sameSign = flipBit(diffSign);
		// 返回A的情况: ab同号则不会溢出, 则直接根据C的符号判断是否返回A, ab异号则若a大于0就返回A, 否则返回B
		int returnA = diffSign * signA + sameSign * signC;
		// 否则就返回B
		int returnB = flipBit(returnA);
		return a * returnA + b * returnB;
	}

	// 在a不是0就是1的请款下, 反转这个数
	static int flipBit(int a) {
		return a ^ 1;
	}

	// a如果为负数返回0, 非负数返回1
	static int getSign(int a) {
		return flipBit(a >> (sizeof(int) * 8 - 1));
	}

	// 判断一个32位正数是不是2的幂、4的幂
	// 二进制下, 2的幂只有一位为1:
	//  1: 1
	//  2: 10
	//  4: 100
	// 因为乘2就是左移一位
	static bool isPowerOf2(int num) {
		// return num == (num & (~num + 1));
		// 或者更快的
		return (num & (num - 1)) == 0;
	}

	// 乘4相当于左移2位
	static bool isPowerOf4(int num) {
		// 依旧是只有1一个1, 同时1只能在偶数位, 所以就和...0101010101比较
		return (num & (num - 1)) == 0 && (num & 0x55555555) != 0;
	}

	// 给定两个有符号32位整数a和b，不能使用算术运算符，分别实现a和b的加、减、乘、除运算
	// 【要求】如果给定a、b执行加减乘除的运算结果就会导致数据的溢出，那么你实现的函数不必对此负责，除此之外请保证计算过程不发生溢出
	static std::tuple<int, int, int, int> bitCalculation(int num1, int num2) {
		return {bitAdd(num1, num2), bitSub(num1, num2), bitMulti(num1, num2), bitDiv(num1, num2)};
	}

	// 异或就是无进位相加, 因此通过与得到进位信息后再次异或, 直到没有进位信息
	static int bitAdd(int num1, int num2) {
		int sum = num1;
		while (num2 != 0) {
			sum = num1 ^ num2;
			num2 = (num1 & num2) << 1;
			num1 = sum;
		}
		return sum;
	}

	static int bitNeg(int num) {
		return bitAdd(~num, 1);
	}

	// a - b = a + (-b),
	static int bitSub(int num1, int num2) {
		return bitAdd(num1, bitNeg(num2));
	}

	// 二进制乘法, 每次左移一位
	static int bitMulti(int num1, int num2) {
		int sum = 0;
		while (num2 != 0) {
			if ((num2 & 1) != 0)
				sum = bitAdd(sum, num1);
			num1 <<= 1;
			num2 = (signed) ((unsigned) num2 >> 1);
		}
		return sum;
	}

	static bool isNeg(int n) {
		return n < 0;
	}

	static bool negNum(int n) {
		return bitMulti(n, -1);
	}

	static int bitDiv(int num1, int num2) {
		int x = isNeg(num1) ? negNum(num1) : num1;
		int y = isNeg(num2) ? negNum(num2) : num2;
		int res = 0;
		for (int i = 31; i > -1; i = bitSub(i, 1)) {
			if ((x >> i) >= y) {
				res |= (1 << i);
				x = bitSub(x, y << i);
			}
		}
		return isNeg(num1) ^ isNeg(num2) ? negNum(res) : res;
	}
};


int main(int argc, char *argv[]) {
	cout << "BitMax1: " << BitOperation::bitCompareMax1(12, 3) << "\n";
	cout << "BitMax2: " << BitOperation::bitCompareMax2(12, 3) << "\n";
	cout << std::boolalpha;
	cout << "IsPowerOf2: " << BitOperation::isPowerOf2(2) << "\n";
	cout << "IsPowerOf2: " << BitOperation::isPowerOf2(32) << "\n";
	cout << "IsPowerOf2: " << BitOperation::isPowerOf2(15) << "\n";
	std::tuple<int, int, int, int> result = BitOperation::bitCalculation(12, 3);
	cout << "BitCalculation: " << std::get<0>(result) << " " << std::get<1>(result) << " " << std::get<2>(result) << " " << std::get<3>(result) << "\n";
	return 0;
}