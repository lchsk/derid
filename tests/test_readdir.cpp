#include <unordered_set>

#include "gtest/gtest.h"

#define LOGURU_IMPLEMENTATION 1
#include "../src/include/loguru.hpp"

#include "../src/buffer.hpp"

TEST(Buffer, readdir)
{
    derid::buffer b;

    b.read_dir("./tests/test_data");

    ASSERT_EQ(b.entries.size(), 6);

    std::set<std::string> names;

    for (const auto& entry : b.entries) {
        names.insert(entry.name);
    }

    ASSERT_EQ(names, std::set<std::string>({".", "..", "file1", "dir1", "dir2", "dir3"}));
}

int main (int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
