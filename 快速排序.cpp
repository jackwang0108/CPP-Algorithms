#include <vector>
#include <iostream>

#include "printVec.h"

using std::cout;
using std::vector;

template <typename T>
size_t partition(vector<T> &vec, size_t left, size_t right)
{
    if (left == right)
        return left;
    T pivot = vec[left];
    std::swap(vec[left], vec[right]);
    int lessRegionPos = left - 1, currValPos;
    for (currValPos = left; currValPos <= right; currValPos++)
    {
        if (vec[currValPos] < pivot)
        {
            lessRegionPos += 1;
            if (lessRegionPos < currValPos)
                std::swap(vec[currValPos], vec[lessRegionPos]);
        }
    }
    std::swap(vec[lessRegionPos + 1], vec[right]);
    return lessRegionPos + 1;
}

template <typename T>
vector<T> &quickSort(vector<T> &vec, int left, int right)
{
    if (left > right)
        return vec;
    size_t pivotPos = partition(vec, left, right);
    if (pivotPos > left)
        quickSort(vec, left, pivotPos - 1);
    if (pivotPos < right)
        quickSort(vec, pivotPos + 1, right);
    return vec;
}

int main(int argc, char *argv[])
{
    vector<int> nums{5, 9, 2, 1, 3, 4, 5, 6, 0, 8};
    quickSort<int>(nums, 0, nums.size() - 1);
    cout << nums;
    return 0;
}