#include <gtest/gtest.h>
#include "partitioning_formatting.h"

// Test the createPartition function
TEST(PartitioningFormattingTest, CreatePartitionSuccess) {
    auto result = PartitioningFormatting::createPartition("sda", PartitionTableType::GPT, 100.0);
    EXPECT_TRUE(result.success);
    EXPECT_EQ(result.details, "Partition created successfully.");
}

// Test the formatPartition function
TEST(PartitioningFormattingTest, FormatPartitionSuccess) {
    auto result = PartitioningFormatting::formatPartition("sda1", FileSystemType::EXT4);
    EXPECT_TRUE(result.success);
    EXPECT_EQ(result.details, "Partition formatted successfully.");
}
