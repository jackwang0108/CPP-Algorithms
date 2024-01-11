#include <vector>
#include <iostream>

#define DEBUG

#include "printVec.h"

using std::cout;
using std::vector;

template <typename T>
vector<T> &insertSort(vector<T> &vec)
{
    if (vec.size() == 0 || vec.size() == 1)
        return vec;

    int n = vec.size();
    for (int toSortIdx = 1; toSortIdx < n; ++toSortIdx)
    {
        T numToInsert = vec[toSortIdx];
        int sortedIndex = toSortIdx - 1;
        while (sortedIndex >= 0 && vec[sortedIndex] > numToInsert)
        {
            vec[sortedIndex + 1] = vec[sortedIndex];
            --sortedIndex;
        }
        vec[sortedIndex + 1] = numToInsert;
    }

    return vec;
}

int main(int argc, char *argv[])
{
    vector<int> nums{9, 2, 2, 1, 3, 4, 5, 6, 0, 8};
    insertSort<int>(nums);
    cout << nums;
    return 0;
}