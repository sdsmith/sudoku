#pragma once

#include "types.h"
#include <algorithm>
#include <bitset>
#include <cassert>
#include <limits>
#include <vector>

// @speed: potentially faster than unordered insert in cell possibilities?
template <typename T>
typename std::vector<T>::iterator insert_sorted(std::vector<T>& v, const T& el)
{
    return v.insert(std::upper_bound(v.begin(), v.end(), el), el);
}

template <size_t N>
u8 first_unset_bit(const std::bitset<N>& bitset)
{
    assert(N < std::numeric_limits<u8>::max());

    for (u8 i = 0; i < static_cast<u8>(N); ++i) {
        if (!bitset[i]) {
            return i;
        }
    }

    assert(false);
}
