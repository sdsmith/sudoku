#pragma once

#include <algorithm>
#include <bitset>
#include <cassert>
#include <vector>

// @speed: potentially faster than unordered insert in cell possibilities?
template <typename T>
typename std::vector<T>::iterator insert_sorted(std::vector<T>& v, const T& el)
{
    return v.insert(std::upper_bound(v.begin(), v.end(), el), el);
}

template <size_t N>
size_t first_unset_bit(const std::bitset<N>& bitset)
{
    for (size_t i = 0; i < N; ++i) {
        if (!bitset[i]) {
            return i;
        }
    }

    assert(false);
}
