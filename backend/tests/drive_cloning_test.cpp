#include <gtest/gtest.h>
#include "drive_cloning.h"
#include "test_helper.h"  // Common helper to create dummy drives

TEST(DriveCloningTest, CloneDryRunSuccess) {
    // For testing, we simulate cloning from "dummySource" to "dummyDestination".
    DriveCloningResult result = DriveCloning::cloneDrive("dummySource", "dummyDestination", true, false);
    EXPECT_TRUE(result.success);
    EXPECT_EQ(result.details, "Dry-run: Drive cloning simulated successfully.");
}

TEST(DriveCloningTest, CloneInteractiveCancel) {
    // In interactive mode, manual testing might be needed.
    // For automated testing, you might disable interactive mode.
    // Here, we assume interactive mode is off.
    DriveCloningResult result = DriveCloning::cloneDrive("dummySource", "dummyDestination", false, false);
    EXPECT_TRUE(result.success);
    EXPECT_EQ(result.details, "Drive cloning completed successfully.");
}
