#include "types.h"
#include <cassert>
#include <fstream>
#include <bitset>
#include <cstdio>
#include <string>
#include <vector>

template<class T>
class Grid {
private:
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

    [[nodiscard]] s32 length() const { return static_cast<s32>(m_grid.size()); }
    [[nodiscard]] s32 width() const { return static_cast<s32>(m_grid[0].size()); }

    [[nodiscard]] T get(s32 x, s32 y) const
    {
        assert(x >= 0);
        assert(y >= 0);
        return m_grid[static_cast<u32>(y)][static_cast<u32>(x)];
    }

    T& get(s32 x, s32 y)
    {
        assert(x >= 0);
        assert(y >= 0);
        return m_grid[static_cast<u32>(y)][static_cast<u32>(x)];
    }

    void set(s32 x, s32 y, T val)
    {
        assert(x >= 0);
        assert(y >= 0);
        m_grid[static_cast<u32>(y)][static_cast<u32>(x)] = val;
    }
};

constexpr s32 grid_l = 9;
constexpr s32 grid_w = 9;
constexpr s32 square_per_box = 3;
constexpr s32 box_l = grid_l / square_per_box;
constexpr s32 box_w = grid_w / square_per_box;
static_assert(box_l == box_w, "sudoku grid should be square");
static_assert(box_w == square_per_box, "sudoku grid mis-shapen");
static_assert(grid_w / 3 == square_per_box, "sudoku grid mis-shapen");
#define LOG_ERROR(...) fprintf(stderr, "ERROR: " __VA_ARGS__);

void load_file(Grid<s8>& grid, const char filename[]) {
    std::ifstream fs;
    fs.open(filename, std::ios_base::in);
    assert(fs.is_open());

    std::string line;
    s32 y = 0;
    while (std::getline(fs, line) && y < grid_l) {
        if (line.size() > grid_w) {
            LOG_ERROR("line longer than expected: size %zu, expected %d\n", line.size(), grid_w);
            exit(1);
        }
        for (s32 i = 0; i < grid_w; ++i) {
            const s8 n = line[static_cast<size_t>(i)] - '0';
            if (n < 0 || n > 9) {
                LOG_ERROR("invalid grid value: %d", n);
                exit(1);
            }
            grid.set(i, y, line[static_cast<size_t>(i)] - '0');
        }
        ++y;
    }

    if (y < grid_l) {
        LOG_ERROR("file does not have enough rows: found %d, expected %d", y, grid_l);
        exit(1);
    }
    if (!fs.eof()) {
        LOG_ERROR("file contains additional junk");
        exit(1);
    }
}

constexpr bool should_draw_grid_divider(s32 i) {
    static_assert(grid_w == grid_l, "sudoku grid should be square");
    return i % 3 == 2 && i != 0 && i < grid_w - 1;
}

void print_grid(const Grid<s8>& grid) {
    std::string s;
    for (s32 y = 0; y < 9; ++y) {
        for (s32 x = 0; x < 9; ++x) {
            const s8 n = grid.get(x, y);
            s += '0' + n;
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


constexpr s32 box_number(s32 box_x, s32 box_y) {
    // report 1-indexed
    return box_y * box_w + box_x + 1;
}

void validate_grid(const Grid<s8>& grid) {
    // NOTE(sdsmith): report rows/columns/boxes as 1-indexed

    constexpr s32 expected_sum = 45; // sum(1..9)

    // check row
    for (s32 y = 0; y < grid_l; ++y) {
        s32 sum = 0;
        std::bitset<grid_w> nums;
        for (s32 x = 0; x < grid_w; ++x) {
            const s8 n = grid.get(x, y);
            sum += n;
            if (nums.test(static_cast<u8>(n - 1))) {
                LOG_ERROR("multiple %ds in row %d\n", n, y + 1);
            } else {
                nums.set(static_cast<u8>(n - 1));
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
            const s8 n = grid.get(x, y);
            sum += n;
            if (nums.test(static_cast<u8>(n - 1))) {
                LOG_ERROR("multiple %ds in column %d\n", n, x + 1);
            } else {
                nums.set(static_cast<u8>(n - 1));
            }
        }

        if (sum != expected_sum) {
            LOG_ERROR("column %d invalid\n", x + 1);
        }
    }

    // check box
    for (s32 box_y = 0; box_y < box_w; ++box_y) {
        for (s32 box_x = 0; box_x < box_l; ++box_x) {
            const s32 x_offset = box_x * square_per_box;
            const s32 y_offset = box_y * square_per_box;

            s32 sum = 0;
            std::bitset<grid_l * grid_w> nums;
            for (s32 y = y_offset; y < y_offset + square_per_box; ++y) {
                for (s32 x = x_offset; x < x_offset + square_per_box; ++x) {
                    const s8 n = grid.get(x, y);
                    sum += n;
                    if (nums.test(static_cast<u8>(n - 1))) {
                        LOG_ERROR("multiple %d in box %d\n", n, box_number(box_x, box_y));
                    } else {
                        nums.set(static_cast<u8>(n - 1));
                    }
                }
            }

            if (sum != expected_sum) {
                LOG_ERROR("box %d invalid\n", box_number(box_x, box_y));
            }
        }
    }

}

int main(int argc, char* argv[]) {

    if (argc != 2) {
        printf("Usage: sudoku <grid_file>\n");
        return 1;
    }
    const char* grid_file = argv[1];

    Grid<s8> grid(grid_l, grid_w);
    load_file(grid, grid_file);
    print_grid(grid);
    validate_grid(grid);

    return 0;
}
