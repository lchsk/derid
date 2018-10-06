#include <unordered_set>

#include "gtest/gtest.h"

#include "../src/buffer.hpp"

TEST(Buffer, readdir) {
  derid::buffer buffer;

  buffer.read_dir("../tests/test_data");

  ASSERT_EQ(buffer.entries.size(), 6);

  std::set<std::string> names;

  for (const auto &entry : buffer.entries) {
    names.insert(entry.name);
  }

  ASSERT_EQ(names,
            std::set<std::string>({
                ".", "..", "test ą 1 2 3", "dir 1 Ś ó a", "dir2", "dir dir 4",
            }));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
