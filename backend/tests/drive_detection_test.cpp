#include <gtest/gtest.h>
#include "drive_detection.h"

TEST(DriveDetectionTest, ReturnsDrives) {
    // The function should not throw and should return a vector (even if empty).
    EXPECT_NO_THROW({
        auto drives = DriveDetection::getDrives();
        // Optionally, print JSON output for debugging.
        for (const auto &drive : drives) {
            std::cout << drive.toJson() << std::endl;
        }
    });
}
