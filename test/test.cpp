#include "gtest/gtest.h"
#include "sudoku_input.h"

TEST(SudokuInputTest, AcceptedGridCharacters)
{
    for (char c = '0'; c <= '9'; ++c) {
        EXPECT_TRUE(is_accepted_character(c));
    }

    EXPECT_TRUE(is_accepted_character(blank));
}

TEST(CellTest, Construction)
{
    Cell c0;
    EXPECT_EQ(c0.c, blank);
    EXPECT_TRUE(c0.possibilities.empty());

    Cell c1('0');
    EXPECT_EQ(c1.c, '0');
    EXPECT_TRUE(c1.possibilities.empty());
}

TEST(CellTest, Possibilities)
{
    Cell c;

    c.add_possibility('0');
    ASSERT_TRUE(c.is_possibility('0'));
    c.add_possibility('3');
    ASSERT_TRUE(c.is_possibility('3'));
    c.add_possibility('9');
    ASSERT_TRUE(c.is_possibility('9'));
    ASSERT_EQ(c.possibilities.size(), 3U);

    // Item integristy after insert
    ASSERT_TRUE(c.is_possibility('0'));
    ASSERT_TRUE(c.is_possibility('3'));
    ASSERT_TRUE(c.is_possibility('9'));

    // remove item
    c.remove_possibility('3');
    ASSERT_TRUE(c.is_possibility('0'));
    ASSERT_FALSE(c.is_possibility('3'));
    ASSERT_TRUE(c.is_possibility('9'));
    ASSERT_EQ(c.possibilities.size(), 2U);

    // remove remainder of items
    c.remove_possibility('0');
    c.remove_possibility('9');
    ASSERT_FALSE(c.is_possibility('0'));
    ASSERT_FALSE(c.is_possibility('3'));
    ASSERT_FALSE(c.is_possibility('9'));
    ASSERT_EQ(c.possibilities.size(), 0U);
}

TEST(GridTest, Construction) {}

TEST(GridTest, GetSet) {}
