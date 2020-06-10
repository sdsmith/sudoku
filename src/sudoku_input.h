#pragma once

#include "sudoku_grid.h"
#include "types.h"

constexpr bool is_accepted_character(char c) noexcept
{
    return ('0' <= c && c <= '9') || c == blank;
}

void load_file(SudokuGrid& grid, const char* filename) noexcept(false);

constexpr bool should_draw_grid_divider(s32 i) noexcept
{
    static_assert(grid_w == grid_l, "sudoku grid should be square");
    return i % 3 == 2 && i != 0 && i < grid_w - 1;
}

void print_grid(const SudokuGrid& grid) noexcept;
