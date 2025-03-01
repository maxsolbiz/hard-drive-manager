#include <gtest/gtest.h>
#include "drive_health_monitor.h"
#include "test_helper.h"  // Changed include from "tests/test_helper.h" to "test_helper.h"

TEST(DriveHealthMonitorTest, HealthMetricsRetrieval) {
    DriveInfo dummy = createDummyDrive("dummyDrive");
    DriveHealthInfo healthInfo = DriveHealthMonitor::getHealthMetrics(dummy);
    EXPECT_EQ(healthInfo.driveName, dummy.name);
    EXPECT_NE(healthInfo.smartStatus, ""); // Should return some SMART status
    EXPECT_GE(healthInfo.overallHealth, 0.0);
    EXPECT_LE(healthInfo.overallHealth, 100.0);
}

TEST(DriveHealthMonitorTest, JsonOutputFormat) {
    DriveInfo dummy = createDummyDrive("dummyDrive");
    DriveHealthInfo healthInfo = DriveHealthMonitor::getHealthMetrics(dummy);
    std::string json = healthInfo.toJson();
    // Check if the JSON string contains expected keys.
    EXPECT_NE(json.find("\"driveName\":\"dummyDrive\""), std::string::npos);
    EXPECT_NE(json.find("\"temperature\":"), std::string::npos);
}
