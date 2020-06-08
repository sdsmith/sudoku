#pragma once

#include "defines.h"
#include "types.h"
#include <array>
#include <vector>


struct Cell {
    char c = blank;
    std::vector<int> possibilities; //< possible numbers

    Cell() {}
    Cell(char c) : c(c) {}

    void add_possibility(char c);
    void remove_possibility(char c);
};

class SudokuGrid; // TODO(sdsmith): fix

class Box {
    // TODO(sdsmith): make the sudoku grid out of boxes since I reference the
    // grid in box chunks most of the time anyway.
    SudokuGrid* grid = nullptr;
    s32 box_x = -1;
    s32 box_y = -1;

public:
    Box(SudokuGrid& grid, s32 box_x, s32 box_y);

    bool contains(char c) noexcept;
    void set(u8 col, u8 row, char c) noexcept;
    Cell& get(u8 col, u8 row) noexcept;
    bool row_contains(u8 row, char c) const noexcept;
    bool col_contains(u8 col, char c) const noexcept;
};

class AdjacentBoxRow {
    std::vector<Box> m_boxes;

public:
    AdjacentBoxRow(std::vector<Box>&& boxes);
    bool contains(u8 row, char c) const noexcept;
};

class AdjacentBoxCol {
    std::vector<Box> m_boxes;

public:
    AdjacentBoxCol(std::vector<Box>&& boxes);
    bool contains(u8 row, char c) const noexcept;
};
