#include "types.h"
#include <cassert>
#include <fstream>
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
    static_assert(grid_w == grid_l, "sudoku grid should be a square");
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

int main(int argc, char* argv[]) {

    if (argc != 2) {
        printf("Usage: sudoku <grid_file>\n");
        return 1;
    }
    const char* grid_file = argv[1];

    Grid<s8> grid(grid_l, grid_w);
    load_file(grid, grid_file);
    print_grid(grid);

    return 0;
}
