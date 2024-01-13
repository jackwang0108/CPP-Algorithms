#include <vector>
#include <iostream>

using std::cout;
using std::endl;
using std::vector;

template <typename T>
std::ostream &operator<<(std::ostream &os, vector<T> &vec)
{
    for (auto &v : vec)
        os << v << " ";
    os << endl;
    return os;
}

class Sorts
{
public:
    // O(N^2)
    template <typename T>
    static vector<T> &selectSort(vector<T> &vec)
    {
        for (int minRegion = 0; minRegion < vec.size(); minRegion++)
        {
            int minIdx = minRegion;
            for (int testIdx = minRegion + 1; testIdx < vec.size(); testIdx++)
                if (vec[minIdx] > vec[testIdx])
                    minIdx = testIdx;
            std::swap(vec[minRegion], vec[minIdx]);
        }
        return vec;
    }

    // O(N^2)
    template <typename T>
    static vector<T> &bubbleSort(vector<T> &vec)
    {
        for (int maxIdx = vec.size() - 1; maxIdx >= 0; maxIdx--)
            for (int testIdx = 0; testIdx < maxIdx; testIdx++)
                if (vec[testIdx] > vec[testIdx + 1])
                    std::swap(vec[testIdx], vec[testIdx + 1]);
        return vec;
    }

    template <typename T>
    static vector<T> &insertSort(vector<T> &vec)
    {
        for (int insertIdx = 1; insertIdx < vec.size(); insertIdx++)
            for (int testIdx = insertIdx - 1; testIdx >= 0 && vec[testIdx] > vec[testIdx + 1]; testIdx--)
                std::swap(vec[testIdx], vec[testIdx + 1]);
        return vec;
    }
};

class BitXor
{
public:
    template <typename T>
    static vector<T> &swap(vector<T> &vec, int i, int j)
    {
        if (i == j)
            return vec;
        vec[i] = vec[i] ^ vec[j];
        vec[j] = vec[i] ^ vec[j];
        vec[i] = vec[i] ^ vec[j];
        return vec;
    }

    template <typename T>
    static T countOneOnce(vector<T> &vec)
    {
        T result = 0;
        for (auto c : vec)
            result ^= c;
        return result;
    }

    // 使得 (a^b)^c = 0的数字一定是b, 所以找到了最后一个数字
    template <typename T>
    static std::pair<T, T> countTwoOnce1(vector<T> &vec)
    {
        T tmp = 0;
        std::pair<T, T> ans;
        for (auto c : vec)
            tmp ^= c;
        ans.first = tmp;
        for (auto c : vec)
        {
            tmp ^= c;
            if (tmp == 0)
                ans.second = c;
        }
        ans.first ^= ans.second;
        return ans;
    }

    // a, b至少有一位不同, 所以根据最低的这一位把数组分成两组数, a和b在不同的组中, 然后分组异或就可以得到其中一个
    template <typename T>
    static std::pair<T, T> countTwoOnce2(vector<T> &vec)
    {
        T tmp = 0;
        for (auto c : vec)
            tmp ^= c;
        // 获得最右边的1
        T rightOne = tmp & (~tmp + 1);
        T onlyOne = 0;
        for (auto c : vec)
            if ((c & rightOne) == 0)
                onlyOne ^= c;
        return {tmp ^ onlyOne, onlyOne};
    }
};

class BinarySearch
{
public:
    template <typename T>
    static int binarySearch(vector<T> &vec, int left, int right, T num)
    {
        if (left > right)
            return -1;
        int mid = (left + right) / 2;
        if (vec[mid] == num)
            return mid;
        int lresult = binarySearch(vec, left, mid - 1, num);
        int rresult = binarySearch(vec, mid + 1, right, num);
        if (lresult != -1)
            return lresult;
        else if (rresult != -1)
            return rresult;
        else
            return -1;
    }

    template <typename T>
    static int findLeftmost(vector<T> &vec, T num)
    {
        int idx = binarySearch(vec, 0, vec.size(), num);
        if (idx == -1)
            return -1;
        while (idx >= 0 && vec[idx] == num && vec[idx - 1] == num)
            idx--;
        return idx;
    }

    template <typename T>
    static int findOneLocalMinima(vector<T> &vec, int left, int right)
    {
        if (left > right)
            return -1;
        if (vec[left] < vec[left + 1])
            return left;
        if (vec[right - 2] > vec[right - 1])
            return right;
        int mid = (left + right) / 2;
        if (vec[mid - 1] > vec[mid] && vec[mid] < vec[mid + 1])
            return mid;
        int lresult = findOneLocalMinima(vec, left, mid - 1);
        int rresult = findOneLocalMinima(vec, mid + 1, right);
        if (lresult != -1)
            return lresult;
        else if (rresult != -1)
            return rresult;
        else
            return -1;
    }
};

int main(int argc, char *argv[])
{
    vector<int> vec{9, -2, -1, 9, 2, 0, -3, 5, -6};
    cout << Sorts::selectSort(vec);
    cout << Sorts::bubbleSort(vec);
    cout << Sorts::insertSort(vec);

    vector<int> nums{1, 2, 3, 4, 5};
    cout << BitXor::swap(nums, 0, 1);

    vector<int> oneNumOnce{1, 1, 2, 2, 3};
    cout << oneNumOnce;
    cout << "oneNumOnce: " << BitXor::countOneOnce(oneNumOnce) << endl;

    vector<int> twoNumOnce{1, 1, 2, 2, 3, 4};
    cout << twoNumOnce;
    std::pair<int, int> result1 = BitXor::countTwoOnce1(twoNumOnce);
    std::pair<int, int> result2 = BitXor::countTwoOnce2(twoNumOnce);
    cout << "twoNumOnce: " << result1.first << " " << result1.second << endl;
    cout << "twoNumOnce: " << result2.first << " " << result2.second << endl;

    vector<int> findNums{1, 2, 3, 4, 5, 6, 7, 8, 9};
    cout << findNums;
    int idx = BinarySearch::binarySearch(findNums, 0, findNums.size(), 2);
    cout << "Find Number: " << findNums[idx] << ", idx: " << idx << endl;

    vector<int> findLeftMost{1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 5, 5, 5};
    cout << findLeftMost;
    int idx1 = BinarySearch::findLeftmost(findLeftMost, 3);
    cout << "Find Leftmost: " << findLeftMost[idx1] << ", idx: " << idx1 << endl;

    vector<int> findOneLocalMinima{5, 4, 3, 2, 1, 2, 3, 4};
    cout << findOneLocalMinima;
    int idx2 = BinarySearch::findOneLocalMinima(findOneLocalMinima, 0, findOneLocalMinima.size());
    cout << "Find findOneLocalMinima: " << findOneLocalMinima[idx2] << ", idx: " << idx2 << endl;
    return 0;
}