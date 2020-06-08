#pragma once

#include "box.h"
#include "defines.h"
#include "grid.h"
#include "types.h"
#include <utility>


/*
  Box numbering scheme:
  +---+---+---+
  | 1 | 2 | 3 |
  +---+---+---+
  | 4 | 5 | 6 |
  +---+---+---+
  | 7 | 8 | 9 |
  +---+---+---+
 */

struct Position {
    s32 x = 0;
    s32 y = 0;

    constexpr Position(s32 x, s32 y) : x(x), y(y) {}
};

constexpr Position box_position(s32 box_num) noexcept {
    const s32 box_x = box_num % side_len;
    const s32 box_y = box_num / side_len;
    return Position(box_x, box_y);
}

constexpr s32 box_number(s32 box_x, s32 box_y) noexcept {
    return box_y * box_w + box_x;
}


class SudokuGrid : public Grid<Cell> {
public:
    SudokuGrid(s32 length, s32 width);
    Box get_box(s32 box_num);
    AdjacentBoxRow get_adj_box_row(s32 box_num);
    AdjacentBoxCol get_adj_box_col(s32 box_num);
};
