#include "sudoku_input.h"

#include <cassert>
#include <fstream>
#include <string>

void load_file(SudokuGrid& grid, const char* filename) noexcept(false)
{
    std::ifstream fs;
    fs.open(filename, std::ios_base::in);
    assert(fs.is_open());

    std::string line;
    s32 y = 0;
    while (std::getline(fs, line) && y < grid_l) {
        if (line.size() > grid_w) {
            LOG_ERROR("line longer than expected: size %zu, expected %d\n",
                      line.size(), grid_w);
            exit(1);
        }
        for (s32 i = 0; i < grid_w; ++i) {
            const char c = line[static_cast<u32>(i)];
            if (!is_accepted_character(c)) {
                LOG_ERROR("invalid grid value: %c\n", c);
                exit(1);
            }
            grid.set(i, y, line[static_cast<u32>(i)]);
        }
        ++y;
    }

    if (y < grid_l) {
        LOG_ERROR("file does not have enough rows: found %d, expected %d\n", y,
                  grid_l);
        exit(1);
    }
    if (!fs.eof()) {
        LOG_ERROR("file contains additional junk\n");
        exit(1);
    }
}

void print_grid(const SudokuGrid& grid) noexcept
{
    std::string s;
    for (s32 y = 0; y < 9; ++y) {
        for (s32 x = 0; x < 9; ++x) {
            const char c = grid.get(x, y).c;
            s += c;
            s += ' ';

            if (should_draw_grid_divider(x)) {
                s += "| ";
            }
        }
        s += '\n';

        if (should_draw_grid_divider(y)) {
            s += "------+-------+------\n";
        }
    }

    assert(s.back() == '\n');
    printf("%s", s.c_str());
}
