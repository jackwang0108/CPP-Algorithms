#include <queue>
#include <random>
#include <vector>
#include <iostream>
#include <algorithm>

using std::cout;
using std::endl;
using std::priority_queue;
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

    // O(N^2)
    template <typename T>
    static vector<T> &insertSort(vector<T> &vec)
    {
        for (int insertIdx = 1; insertIdx < vec.size(); insertIdx++)
            for (int testIdx = insertIdx - 1; testIdx >= 0 && vec[testIdx] > vec[testIdx + 1]; testIdx--)
                std::swap(vec[testIdx], vec[testIdx + 1]);
        return vec;
    }

    // master 公式, a=2, b=2, d=1, log(b,a) =1 == 1, 复杂度 O(NlogN)
    template <typename T>
    static vector<T> &mergeSort(vector<T> &vec, int left, int right)
    {
        if (left == right)
            return vec;
        int mid = left + ((right - left) >> 1);
        mergeSort(vec, left, mid);
        mergeSort(vec, mid + 1, right);
        merge<int>(vec, left, mid, right);
        return vec;
    }

    template <typename T>
    static void merge(vector<T> &vec, int left, int mid, int right)
    {
        vector<T> merged(right - left + 1, 0);
        int leftIdx = left, rightIdx = mid + 1, mergeIdx = 0;
        while (leftIdx <= mid && rightIdx <= right)
            merged[mergeIdx++] = vec[leftIdx] < vec[rightIdx] ? vec[leftIdx++] : vec[rightIdx++];
        while (leftIdx <= mid)
            merged[mergeIdx++] = vec[leftIdx++];
        // ! 因为要用到right的值, 所以调用的时候要传入vec.size() - 1
        while (rightIdx <= right)
            merged[mergeIdx++] = vec[rightIdx++];
        std::copy(merged.begin(), merged.end(), vec.begin() + left);
    }

    // 结合荷兰国旗, 实现快速排序, O(NlogN), 节省空间
    template <typename T>
    static vector<T> &quickSort(vector<T> &vec, int left, int right)
    {
        if (vec.size() == 0 || vec.size() == 1 || left >= right)
            return vec;
        int mid = partition(vec, left, right);
        quickSort(vec, left, mid - 1);
        quickSort(vec, mid + 1, right);
        return vec;
    }

    template <typename T>
    static int partition(vector<T> &vec, int left, int right)
    {
        std::swap(vec[left], vec[right]);
        int lessRegion = -1;
        for (int i = left; i < right; i++)
        {
            if (vec[i] <= vec[right])
                std::swap(vec[++lessRegion + left], vec[i]);
        }
        std::swap(vec[++lessRegion + left], vec[right]);
        return left + lessRegion;
    }

    // 堆排序, 使用大根堆, 时间复杂度O(NlogN), heapInsert和heapify都是logN
    // 空间复杂度 O(1)
    template <typename T>
    static vector<T> &heapSort(vector<T> &vec)
    {
        // heapInsert复杂度高, 可以直接从最后一个节点开始heapify
        // for (int i = 0; i < vec.size(); i++)
        //     heapInsert(vec, i);
        for (int i = vec.size() - 1; i >= 0; i--)
            heapify(vec, i, vec.size());
        int heapSize = vec.size();
        std::swap(vec[0], vec[--heapSize]);
        while (heapSize > 0)
        {
            heapify(vec, 0, heapSize);
            std::swap(vec[0], vec[--heapSize]);
        }

        return vec;
    }

    template <typename T>
    static void heapInsert(vector<T> &vec, int idx)
    {
        while (vec[idx] > vec[(idx - 1) / 2])
        {
            std::swap(vec[idx], vec[(idx - 1) / 2]);
            idx = (idx - 1) / 2;
        }
    }

    template <typename T>
    static void heapify(vector<T> &vec, int idx, int heapSize)
    {
        int left = 2 * idx + 1;
        while (left < heapSize)
        {
            int largerChildIdx = left + 1 < heapSize && vec[left + 1] > vec[left] ? left + 1 : left;
            int largestIdx = vec[idx] > vec[largerChildIdx] ? idx : largerChildIdx;
            if (idx == largestIdx)
                break;
            std::swap(vec[idx], vec[largestIdx]);
            idx = largestIdx;
            left = 2 * idx + 1;
        }
    }

    template <typename T>
    static vector<T> &radixSort(vector<T> &vec)
    {
        int max = vec[0];
        for (int i = 0; i < vec.size(); i++)
            max = std::max(max, vec[i]);
        int digits = 0;
        while (max > 0)
        {
            max /= 10;
            digits++;
        }

        int radix = 10;
        vector<T> bucket(vec.size(), 0);
        for (int d = 0; d < digits; d++)
        {
            vector<T> count(radix, 0);
            // 倒入桶中
            for (int i = 0; i < vec.size(); i++)
                count[getDigit(vec[i], d)]++;
            // 处理为前缀和
            for (int i = 1; i < radix; i++)
                count[i] = count[i] + count[i - 1];
            // 从桶中倒出来
            for (int i = vec.size() - 1; i >= 0; i--)
                bucket[--count[getDigit(vec[i], d)]] = vec[i];
            // 复制回去
            for (int i = 0; i < vec.size(); i++)
                vec[i] = bucket[i];
        }

        return vec;
    }

    static int getDigit(int num, int digit)
    {
        while (digit-- > 0)
            num /= 10;
        return num % 10;
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
        // ! 递归左侧一般不要 mid-1, 直接 left ~ mid这个范围即可, 否则需要判断是否溢出
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

class Problems
{
public:
    // 系统上的递归会自动调用函数栈
    // 递归问题时间复杂度分析使用 master 公式
    // T(N) = a * T(N/b) + O(N^d)
    //  - T(N): 母问题的规模
    //  - T(N/b): 子问题的规模, 即子问题是母问题的N/b
    //  - a: 子问题被调用了多少次
    //  - O(N^d): 除了递归调用以外剩下操作的时间复杂度
    // 所以对于下面的方法, T(N) = 2*T(N/2) + O(1), 即 a=2, b=2, d=0
    // 根据master公式:
    //  * log(b,a) > d,   则时间复杂度为 O(N^log(b,a))
    //  * log(b,a) = d,   则时间复杂度为 O(N^d * logN)
    //  * log(b,a) < d,   则时间复杂度为 O(N^d)
    // 则下面的方法, log(2,2)=1 > 0, 故时间复杂度为O(N), 因此等效于从左到右扫描一遍的时间复杂度
    // 不过空间上的效率不佳
    template <typename T>
    static int getMax(vector<T> &vec, int left, int right)
    {
        if (left >= right)
            return vec[left];
        // (left+right)可能溢出, 右移可以防止溢出
        int mid = left + ((right - left) >> 1);
        int leftMax = getMax(vec, left, mid);
        int rightMax = getMax(vec, mid + 1, right);
        return std::max(leftMax, rightMax);
    }

    // 小和问题
    // 在一个数组中，每一个数左边比当前数小的数累加起来，叫做这个数组的小和。
    // 求一个数组的小和。例子: [1,3,4,2,5], 1左边比1小的数，没有;
    // 3左边比3小的数，1; 4左边比4小的数，1、3; 2左边比2小的数，1;
    // 5左边比5小的数，1、3、4、2;
    // 所以小和为1+1+3+1+1+3+4+2=16
    template <typename T>
    static int smallSum(vector<T> &vec)
    {
        if (vec.size() == 0 || vec.size() == 1)
            return 0;
        return processSmallSum(vec, 0, vec.size() - 1);
    }

    template <typename T>
    static int processSmallSum(vector<T> &vec, int left, int right)
    {
        if (left == right)
            return 0;
        int mid = left + ((right - left) >> 1);
        return processSmallSum(vec, left, mid) + processSmallSum(vec, mid + 1, right) + mergeSmallSum(vec, left, mid, right);
    }

    template <typename T>
    static int mergeSmallSum(vector<T> &vec, int left, int mid, int right)
    {
        int sum = 0;
        int leftIdx = left, rightIdx = mid + 1, mergedIdx = 0;
        vector<T> merged(right - left + 1, 0);
        while (leftIdx <= mid && rightIdx <= right)
        {
            if (vec[leftIdx] < vec[rightIdx])
            {
                sum += (right - rightIdx + 1) * vec[leftIdx];
                merged[mergedIdx++] = vec[leftIdx++];
            }
            else
                merged[mergedIdx++] = vec[rightIdx++];
        }
        while (leftIdx <= mid)
            merged[mergedIdx++] = vec[leftIdx++];
        while (rightIdx <= right)
            merged[mergedIdx++] = vec[rightIdx++];
        std::copy(merged.begin(), merged.end(), vec.begin() + left);
        return sum;
    }

    // 逆序对问题
    // 在一个数组中，左边的数如果比右边的数大，则折两个数构成一个逆序对，请打印所有逆序对。
    template <typename T>
    static void reversedPair(vector<T> &vec)
    {
        if (vec.size() == 0 || vec.size() == 1)
            return;
        processReversedPair(vec, 0, vec.size() - 1);
    }

    template <typename T>
    static void processReversedPair(vector<T> &vec, int left, int right)
    {
        if (left >= right)
            return;
        int mid = left + ((right - left) >> 1);
        processReversedPair(vec, left, mid);
        processReversedPair(vec, mid + 1, right);
        mergeReversedPair(vec, left, mid, right);
    }

    template <typename T>
    static void mergeReversedPair(vector<T> vec, int left, int mid, int right)
    {
        int leftIdx = left, rightIdx = mid + 1, mergedIdx = 0;
        vector<T> merged(right - left + 1, 0);
        while (leftIdx <= mid && rightIdx <= right)
        {
            if (vec[leftIdx] < vec[rightIdx])
                merged[mergedIdx++] = vec[leftIdx++];
            else
            {
                for (int i = leftIdx; i <= mid; i++)
                    cout << " (" << vec[i] << "," << vec[rightIdx] << ") ";
                merged[mergedIdx++] = vec[rightIdx++];
            }
        }
        while (leftIdx <= mid)
            merged[mergedIdx++] = vec[leftIdx++];
        while (rightIdx <= right)
            merged[mergedIdx++] = vec[rightIdx++];
        std::copy(merged.begin(), merged.end(), vec.begin() + left);
    }

    // 荷兰国旗问题 1
    // 给定一个数组arr，和一个数num，请把小于等于num的数放在数组的左边，大于num的数放在数组的右边。
    // 要求额外空间复杂度O(1)，时间复杂度O(N)
    template <typename T>
    static void hollandFlag(vector<T> &vec, int num)
    {
        int lessRegion = -1;
        for (int i = 0; i < vec.size(); i++)
            if (vec[i] <= num)
                std::swap(vec[++lessRegion], vec[i]);
    }

    // 荷兰国旗问题 2
    // 给定一个数组arr，和一个数num，请把小于num的数放在数组的左边，等于num的数放在数组的中间，大于num的数放在数组的右边。
    // 要求额外空间复杂度O(1)，时间复杂度O(N)
    template <typename T>
    static void hollandFlagFull(vector<T> &vec, int num)
    {
        int lessRegion = -1, greatRegion = vec.size();
        for (int i = 0; i < vec.size() && greatRegion > i; i++)
        {
            if (vec[i] < num)
                std::swap(vec[++lessRegion], vec[i]);
            else if (vec[i] > num)
                std::swap(vec[--greatRegion], vec[i]);
        }
    }

    // 堆排序扩展题目
    // 已知一个几乎有序的数组，几乎有序是指，如果把数组排好顺序的话，每个元素移动的距离可以不超过k，并且k相对于数组来说比较小。
    // 请选择一个合适的排序算法针对这个数据进行排序。
    template <typename T>
    static vector<T> &sortedArrayLessK(vector<T> &vec, int k)
    {
        priority_queue<int, vector<int>, std::greater<>> heap;
        for (int i = 0; i < k; i++)
            heap.push(vec[i]);
        int copyIdx = 0;
        for (int i = k; i < vec.size(); i++)
        {
            vec[copyIdx++] = heap.top();
            heap.pop();
            heap.push(vec[i]);
        }
        while (!heap.empty())
        {
            vec[copyIdx++] = heap.top();
            heap.pop();
        }
        return vec;
    }
};

int main(int argc, char *argv[])
{
    auto rng = std::default_random_engine{};

    vector<int> vec{9, -2, -1, 9, 2, 0, -3, 5, -6};
    std::shuffle(vec.begin(), vec.end(), rng);
    cout << "Shuffled: " << vec;
    cout << "After Sorts: " << Sorts::selectSort(vec);
    std::shuffle(vec.begin(), vec.end(), rng);
    cout << "Shuffled: " << vec;
    cout << "After Sorts: " << Sorts::bubbleSort(vec);
    std::shuffle(vec.begin(), vec.end(), rng);
    cout << "Shuffled: " << vec;
    cout << "After Sorts: " << Sorts::insertSort(vec);
    std::shuffle(vec.begin(), vec.end(), rng);
    cout << "Shuffled: " << vec;
    cout << "After Sorts: " << Sorts::mergeSort(vec, 0, vec.size() - 1);
    std::shuffle(vec.begin(), vec.end(), rng);
    cout << "Shuffled: " << vec;
    cout << "After Sorts: " << Sorts::quickSort(vec, 0, vec.size() - 1);
    std::shuffle(vec.begin(), vec.end(), rng);
    cout << "Shuffled: " << vec;
    cout << "After Sorts: " << Sorts::heapSort(vec);

    vector<int> radixNums{6, 11, 25, 30, 100, 57};
    cout << "Shuffled" << radixNums;
    cout << "After Sorts: " << Sorts::radixSort(radixNums);

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

    vector<int> vec1{9, -2, -1, 9, 2, 0, -3, 5, 16};
    cout << vec1;
    int max = Problems::getMax(vec1, 0, vec1.size());
    cout << "Max: " << max << endl;

    vector<int> smallSum{1, 3, 4, 2, 5};
    cout << "Small Sum: " << Problems::smallSum(smallSum) << endl;

    vector<int> reversedPair{5, 4, 3, 2, 1};
    cout << reversedPair;
    cout << "All reversed pair: ";
    Problems::reversedPair(reversedPair);
    cout << endl;

    vector<int> hollandFlag1{3, 5, 6, 7, 4, 3, 5, 8};
    cout << hollandFlag1;
    Problems::hollandFlag(hollandFlag1, 5);
    cout << hollandFlag1;
    vector<int> hollandFlag2{3, 5, 6, 7, 4, 3, 5, 8};
    cout << hollandFlag2;
    Problems::hollandFlagFull(hollandFlag2, 5);
    cout << hollandFlag2;

    vector<int> sortedLessK{3, 2, 1, 6, 5, 4, 9, 8, 7};
    cout << sortedLessK;
    cout << "After sorted: " << Problems::sortedArrayLessK(sortedLessK, 3);

    return 0;
}
