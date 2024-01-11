#include <vector>
#include <iostream>

#define DEBUG

#include "printVec.h"

using std::vector;

template <typename T>
vector<T> &selectSort(vector<T> &vec)
{
    if (vec.size() == 0 || vec.size() == 1)
        return vec;

    for (int i = 0; i < vec.size(); i++)
    {
        int currMinPos = i;
        for (int j = i; j < vec.size(); j++)
        {
            if (vec[currMinPos] > vec[j])
                currMinPos = j;
        }
        int temp = vec[i];
        vec[i] = vec[currMinPos];
        vec[currMinPos] = temp;
        debugPrint(std::cout, vec);
    }
    return vec;
}

int main(int argc, char *argv[])
{
    vector<int> nums{9, 2, 2, 1, 3, 4, 5, 6, 0, 8};
    selectSort<int>(nums);
    return 0;
}