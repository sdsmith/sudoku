#include "sudoku_grid.h"


SudokuGrid::SudokuGrid(s32 length, s32 width) : Grid(length, width) {}

Box SudokuGrid::get_box(s32 box_num) {
    const Position p = box_position(box_num);
    return Box(*this, p.x, p.y);
}

AdjacentBoxRow SudokuGrid::get_adj_box_row(s32 box_num) {
    std::vector<Box> v;
    v.reserve(2);
    for (s32 n = 0; n < side_len; ++n) {
        if (n == box_num % side_len) { continue; } // cur box
        const Position p = box_position((box_num % side_len) * grid_w + n);
        v.emplace_back(*this, p.x, p.y);
    }

    assert(v.size() == 2);
    return AdjacentBoxRow(std::move(v));
}

AdjacentBoxCol SudokuGrid::get_adj_box_col(s32 box_num) {
    std::vector<Box> v;
    v.reserve(2);
    for (s32 n = box_num % side_len; n <= 9; n += 3) { // TODO(sdsmith): hardcoded
        if (n == box_num) { continue; } // cur box
        const Position p = box_position(n);
        v.emplace_back(*this, p.x, p.y);
    }

    assert(v.size() == 2);
    return AdjacentBoxCol(std::move(v));
}
