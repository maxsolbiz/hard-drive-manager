#include <gtest/gtest.h>
#include "scanning.h"
#include "test_helper.h"  // Changed include from "tests/test_helper.h" to "test_helper.h"

TEST(ScanningTest, QuickScanSuccess) {
    DriveInfo dummy = createDummyDrive("dummy0");
    ScanResult result = Scanning::scanDrive(dummy, ScanType::QUICK);
    EXPECT_TRUE(result.success);
    EXPECT_EQ(result.driveName, dummy.name);
    EXPECT_EQ(result.details, "Quick scan completed successfully.");
    EXPECT_GT(result.durationSeconds, 0.0);
}

TEST(ScanningTest, DetailedScanSuccess) {
    DriveInfo dummy = createDummyDrive("dummy1");
    ScanResult result = Scanning::scanDrive(dummy, ScanType::DETAILED);
    EXPECT_TRUE(result.success);
    EXPECT_EQ(result.driveName, dummy.name);
    EXPECT_EQ(result.details, "Detailed scan completed successfully with no errors found.");
    EXPECT_GT(result.durationSeconds, 0.0);
}

TEST(ScanningTest, AsyncScanMultipleDrives) {
    std::vector<DriveInfo> drives = { createDummyDrive("dummy0"), createDummyDrive("dummy1") };
    auto results = Scanning::scanAllDrivesAsync(drives, ScanType::QUICK);
    EXPECT_EQ(results.size(), drives.size());
    for (const auto &res : results) {
        EXPECT_TRUE(res.success);
    }
}
