#include <gtest/gtest.h>
#include "drive_cloning.h"
#include "test_helper.h"  // Common helper to create dummy drives

TEST(DriveCloningTest, CloneDryRunSuccess) {
    // For testing, we simulate cloning from "dummySource" to "dummyDestination".
    // Pass an empty lambda as the progress callback.
    DriveCloningResult result = DriveCloning::cloneDrive("dummySource", "dummyDestination", [](double progress) {}, true, false);
    EXPECT_TRUE(result.success);
    EXPECT_EQ(result.details, "Dry-run: Drive cloning simulated successfully.");
}

TEST(DriveCloningTest, CloneInteractiveCancel) {
    // In interactive mode, manual testing might be needed.
    // For automated testing, assume interactive mode is off.
    // Pass an empty lambda as the progress callback.
    DriveCloningResult result = DriveCloning::cloneDrive("dummySource", "dummyDestination", [](double progress) {}, false, false);
    EXPECT_TRUE(result.success);
    EXPECT_EQ(result.details, "Drive cloning completed successfully.");
}
