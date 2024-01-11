#ifndef __PRINTVEC_H
#define __PRINTVEC_H

#include <vector>
#include <iostream>

template <typename T>
std::ostream &operator<<(std::ostream &os, std::vector<T> vec)
{
    for (auto &i : vec)
        os << i << " ";
    os << std::endl;
    return os;
}

template <typename T>
std::vector<T> &debugPrint(std::ostream &os, std::vector<T> &vec, bool delim = false)
{
#ifdef DEBUG
    if (delim)
        os << "-------------------------------" << std::endl;
    else
        os << "DEBUG: " << vec;
#endif
    return vec;
}

#endif