#pragma once

#include "types.h"
#include <cstdio>


constexpr char blank = '-';
constexpr s32 side_len = 3;
constexpr s32 grid_l = 9;
constexpr s32 grid_w = 9;
constexpr s32 cell_per_box = 3;
constexpr s32 box_l = grid_l / cell_per_box;
constexpr s32 box_w = grid_w / cell_per_box;
static_assert(box_l == box_w, "sudoku grid should be square");
static_assert(box_w == cell_per_box, "sudoku grid mis-shapen");
static_assert(grid_w / 3 == cell_per_box, "sudoku grid mis-shapen");

#define LOG_ERROR(...) fprintf(stderr, "ERROR: " __VA_ARGS__);
