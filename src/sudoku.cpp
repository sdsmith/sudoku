#include "box.h"
#include "defines.h"
#include "grid.h"
#include "sudoku_grid.h"
#include "sudoku_input.h"
#include "types.h"
#include "utils.h"
#include <algorithm>
#include <bitset>
#include <cassert>
#include <cstdio>
#include <string>
#include <vector>

/**
 * DOC(sdsmith):
 *
 * @return True if possibilities were added.
 */
bool mark_possibilities_if_box_2_free_cells(
    Box& box, char c, const std::bitset<side_len>& row_has_c,
    const std::bitset<side_len>& col_has_c)
{
    // @memory: @speed: don't need a vector. A 2 el stack alloc collection would
    // work.
    std::vector<Cell*> v;
    for (u8 row = 0; row < side_len; ++row) {
        if (row_has_c.test(row)) {
            continue;
        }
        for (u8 col = 0; col < side_len; ++col) {
            if (col_has_c.test(col)) {
                continue;
            }

            Cell& cell = box.get(col, row);
            if (cell.c == blank) {
                if (v.size() > 2) {
                    return false;
                }

                v.push_back(&cell);
            }
        }
    }

    assert(v.size() <= 2);
    if (v.size() == 2) {
        // there were only 2 cells
        for (Cell* cell : v) {
            cell->add_possibility(c);
        }
        return true;
    }
    return false;
}

void solve_grid(SudokuGrid& grid)
{
    // TODO(sdsmith): adapt for every number and every box
    char cur_c = '1';
    Box box = grid.get_box(0);
    AdjacentBoxRow adj_box_row = grid.get_adj_box_row(0);
    AdjacentBoxCol adj_box_col = grid.get_adj_box_col(0);

    if (box.contains(cur_c)) {
        // TODO(sdsmith): skip box
    }

    std::bitset<side_len> row_has_c;
    for (u8 row = 0; row < side_len; ++row) {
        if (adj_box_row.contains(row, cur_c)) {
            row_has_c.set(row);
        }
    }

    std::bitset<side_len> col_has_c;
    for (u8 col = 0; col < side_len; ++col) {
        if (adj_box_col.contains(col, cur_c)) {
            col_has_c.set(col);
        }
    }

    if (row_has_c.count() == side_len - 1 &&
        col_has_c.count() == side_len - 1) {
        // Row+col is enough to determine the value
        box.set(first_unset_bit(col_has_c), first_unset_bit(row_has_c), cur_c);
    } else { // Determine any possibilities
        mark_possibilities_if_box_2_free_cells(box, cur_c, row_has_c,
                                               col_has_c);
    }

    // TODO(sdsmith):
}

void validate_grid(const SudokuGrid& grid)
{
    // NOTE(sdsmith): report rows/columns/boxes as 1-indexed

    constexpr s32 expected_sum = 45; // sum(1..9)

    // check row
    for (s32 y = 0; y < grid_l; ++y) {
        s32 sum = 0;
        std::bitset<grid_w> nums;
        for (s32 x = 0; x < grid_w; ++x) {
            const char c = grid.get(x, y).c;
            if (c == blank) {
                printf("grid is not complete\n");
                return;
            }

            const s32 n = c - '0';
            sum += n;
            if (nums.test(static_cast<u32>(n - 1))) {
                LOG_ERROR("multiple %ds in row %d\n", n, y + 1);
            } else {
                nums.set(static_cast<u32>(n - 1));
            }
        }

        if (sum != expected_sum) {
            LOG_ERROR("row %d invalid\n", y + 1);
        }
    }

    // check column
    for (s32 x = 0; x < grid_w; ++x) {
        s32 sum = 0;
        std::bitset<grid_l> nums;
        for (s32 y = 0; y < grid_l; ++y) {
            const char c = grid.get(x, y).c;
            if (c == blank) {
                printf("grid is not complete\n");
                return;
            }

            const s32 n = c - '0';
            sum += n;
            if (nums.test(static_cast<u32>(n - 1))) {
                LOG_ERROR("multiple %ds in column %d\n", n, x + 1);
            } else {
                nums.set(static_cast<u32>(n - 1));
            }
        }

        if (sum != expected_sum) {
            LOG_ERROR("column %d invalid\n", x + 1);
        }
    }

    // check box
    for (s32 box_y = 0; box_y < box_w; ++box_y) {
        for (s32 box_x = 0; box_x < box_l; ++box_x) {
            const s32 x_offset = box_x * cell_per_box;
            const s32 y_offset = box_y * cell_per_box;

            s32 sum = 0;
            std::bitset<grid_l * grid_w> nums;
            for (s32 y = y_offset; y < y_offset + cell_per_box; ++y) {
                for (s32 x = x_offset; x < x_offset + cell_per_box; ++x) {
                    const char c = grid.get(x, y).c;
                    if (c == blank) {
                        printf("grid is not complete\n");
                        return;
                    }

                    const s32 n = c - '0';
                    sum += n;
                    if (nums.test(static_cast<u32>(n - 1))) {
                        LOG_ERROR("multiple %d in box %d\n", n,
                                  box_number(box_x, box_y) + 1);
                    } else {
                        nums.set(static_cast<u32>(n - 1));
                    }
                }
            }

            if (sum != expected_sum) {
                LOG_ERROR("box %d invalid\n", box_number(box_x, box_y) + 1);
            }
        }
    }

    printf("QED\n");
}

int main(int argc, char* argv[])
try {
    if (argc != 2) {
        printf("Usage: sudoku <grid_file>\n");
        return 1;
    }
    const char* grid_file = argv[1];

    SudokuGrid grid(grid_l, grid_w);
    load_file(grid, grid_file);
    printf("original grid...\n");
    print_grid(grid);
    printf("solving...\n");
    solve_grid(grid);
    printf("final grid...\n");
    print_grid(grid);
    printf("validating...\n");
    validate_grid(grid);

    return 0;
} catch (const std::exception& e) {
    printf("uncaught exception occured: %s\n", e.what());
} catch (...) {
    printf("unknown exception occured\n");
}
