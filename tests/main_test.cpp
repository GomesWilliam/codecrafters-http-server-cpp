#include <gtest/gtest.h>

TEST(MainTest, Fast) {
    EXPECT_EQ(2 + 2, 4);
}

TEST(MainTest, Slow) {
    EXPECT_TRUE(true);
}