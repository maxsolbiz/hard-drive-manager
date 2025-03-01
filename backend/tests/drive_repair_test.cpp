#include <gtest/gtest.h>
#include "drive_repair.h"
#include "test_helper.h"  // Changed include from "tests/test_helper.h" to "test_helper.h"

TEST(DriveRepairTest, RepairDryRunSuccess) {
    DriveInfo dummy = createDummyDrive("dummyDrive");
    // Use dry-run mode to bypass the need for root privileges.
    DriveRepairResult result = DriveRepair::repairDrive(dummy, true, false);
    EXPECT_TRUE(result.success);
    EXPECT_EQ(result.details, "Dry-run: Repair simulated successfully.");
}
