#include <vector>
#include <iostream>

#define DEBUG

#include "printVec.h"

using std::cout;
using std::endl;
using std::vector;

template <typename T>
vector<T> &bubbleSort(vector<T> &vec)
{
    if (vec.size() == 0 || vec.size() == 1)
        return vec;

    T temp;
    for (int i = 0; i < vec.size(); i++)
    {
        for (int j = 0; j < vec.size() - i - 1; j++)
        {
            if (vec[j] > vec[j + 1])
            {
                temp = vec[j];
                vec[j] = vec[j + 1];
                vec[j + 1] = temp;
                debugPrint(cout, vec);
            }
        }
    }
    debugPrint(cout, vec, true);
    return vec;
}

int main(int argc, char *argv[])
{
    vector<int> nums{9, 2, 2, 1, 3, 4, 5, 6, 0, 8};
    bubbleSort<int>(nums);
    return 0;
}