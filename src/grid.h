#pragma once

#include "types.h"
#include <cassert>
#include <vector>


template<class T>
class Grid {
protected:
    std::vector<std::vector<T>> m_grid;

public:
    Grid(s32 length, s32 width)
        : m_grid(static_cast<u32>(length),
                  std::vector<T>(static_cast<u32>(width)))
    {
        assert(length > 0);
        assert(width > 0);
    }

    virtual ~Grid() {}

    [[nodiscard]] s32 length() const noexcept { return static_cast<s32>(m_grid.size()); }
    [[nodiscard]] s32 width() const noexcept { return static_cast<s32>(m_grid[0].size()); }

    [[nodiscard]] T get(s32 x, s32 y) const noexcept
    {
        assert(x >= 0);
        assert(y >= 0);
        return m_grid[static_cast<u32>(y)][static_cast<u32>(x)];
    }

    T& get(s32 x, s32 y) noexcept
    {
        assert(x >= 0);
        assert(y >= 0);
        return m_grid[static_cast<u32>(y)][static_cast<u32>(x)];
    }

    void set(s32 x, s32 y, T val) noexcept
    {
        assert(x >= 0);
        assert(y >= 0);
        m_grid[static_cast<u32>(y)][static_cast<u32>(x)] = val;
    }
};
