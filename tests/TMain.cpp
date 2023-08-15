#include <gtest/gtest.h>

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

/*
// For Reference
int add(int a, int b) {
    return a + b;
}
TEST(AdditionTest, CanAddTwoPositiveNumbers) {
    EXPECT_EQ(add(2, 3), 5);
}
*/