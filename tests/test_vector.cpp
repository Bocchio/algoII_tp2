#include "vector.hpp"
#include <gtest/gtest.h>
namespace {

TEST(VectorTest, Constructor) {
    Vector<int> v;
    EXPECT_EQ(0, v.getSize());
}

}
