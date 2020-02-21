#include <unordered_set>

#include "gtest/gtest.h"

#include "../src/buffer.hpp"

using namespace derid;

TEST(Buffer, readdir) {
    Buffer buffer;

    buffer.ReadDir("../tests/test_data");

    ASSERT_EQ(buffer.Entries().size(), 7);

    std::set<std::string> names;

    for (const auto &entry : buffer.Entries()) {
        names.insert(entry.Name());
    }

    ASSERT_EQ(names, std::set<std::string>({
                         ".",
                         "..",
                         "test ą 1 2 3",
                         "dir 1 Ś ó a",
                         "dir2",
                         "dir dir 4",
                         ".config",
                     }));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
