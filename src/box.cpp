#include "box.h"
#include "defines.h"
#include "sudoku_grid.h"
#include <algorithm>
#include <cassert>

void Cell::add_possibility(char c) { possibilities.push_back(c); }

void Cell::remove_possibility(char c)
{
    const auto it = std::find(possibilities.begin(), possibilities.end(), c);
    assert(it != possibilities.end());
    possibilities.erase(it);
}

Box::Box(SudokuGrid& grid, s32 box_x, s32 box_y)
    : grid(&grid), box_x(box_x), box_y(box_y)
{}

bool Box::contains(char c) noexcept
{
    const s32 x_offset = box_x * cell_per_box;
    const s32 y_offset = box_y * cell_per_box;

    // @speed: calling the getter over and over can't be fast
    for (s32 y = y_offset; y < y_offset + cell_per_box; ++y) {
        for (s32 x = x_offset; x < x_offset + cell_per_box; ++x) {
            if (grid->get(x, y).c == c) {
                return true;
            }
        }
    }

    return false;
}

void Box::set(u8 col, u8 row, char c) noexcept
{
    const s32 x_offset = box_x * cell_per_box;
    const s32 y_offset = box_y * cell_per_box;
    grid->set(x_offset + col, y_offset + row, c);
}

Cell& Box::get(u8 col, u8 row) noexcept
{
    const s32 x_offset = box_x * cell_per_box;
    const s32 y_offset = box_y * cell_per_box;
    return grid->get(x_offset + col, y_offset + row);
}

bool Box::row_contains(u8 row, char c) const noexcept
{
    const s32 x_offset = box_x * cell_per_box;
    const s32 y_offset = box_y * cell_per_box;

    for (s32 x = x_offset; x < x_offset + cell_per_box; ++x) {
        if (grid->get(x, y_offset + row).c == c) {
            return true;
        }
    }

    return false;
}

bool Box::col_contains(u8 col, char c) const noexcept
{
    const s32 x_offset = box_x * cell_per_box;
    const s32 y_offset = box_y * cell_per_box;

    for (s32 y = y_offset; y < y_offset + cell_per_box; ++y) {
        if (grid->get(x_offset + col, y).c == c) {
            return true;
        }
    }

    return false;
}

AdjacentBoxRow::AdjacentBoxRow(std::vector<Box>&& boxes)
    : m_boxes(std::move(boxes))
{}

bool AdjacentBoxRow::contains(u8 row, char c) const noexcept
{
    assert(m_boxes.size() == 2);
    return m_boxes[0].row_contains(row, c) || m_boxes[1].row_contains(row, c);
}

AdjacentBoxCol::AdjacentBoxCol(std::vector<Box>&& boxes)
    : m_boxes(std::move(boxes))
{}

bool AdjacentBoxCol::contains(u8 row, char c) const noexcept
{
    assert(m_boxes.size() == 2);
    return m_boxes[0].col_contains(row, c) || m_boxes[1].col_contains(row, c);
}
