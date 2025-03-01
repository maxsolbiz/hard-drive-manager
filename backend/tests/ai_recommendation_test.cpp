#include <gtest/gtest.h>
#include "ai_recommendation.h"
#include "drive_health_monitor.h"

// We can create dummy health info directly.
TEST(AIRecommendationTest, HealthyDrive) {
    DriveHealthInfo health;
    health.driveName = "dummyDrive";
    health.temperature = 30.0;
    health.readErrorCount = 0;
    health.writeErrorCount = 0;
    health.overallHealth = 95.0;
    health.smartStatus = "PASSED";
    
    std::string recommendation = AIRecommendation::getRecommendation(health);
    EXPECT_NE(recommendation.find("Drive is healthy"), std::string::npos);
}

TEST(AIRecommendationTest, LowHealthDrive) {
    DriveHealthInfo health;
    health.driveName = "dummyDrive";
    health.temperature = 40.0;
    health.readErrorCount = 5;
    health.writeErrorCount = 5;
    health.overallHealth = 75.0;
    health.smartStatus = "FAILED";
    
    std::string recommendation = AIRecommendation::getRecommendation(health);
    EXPECT_NE(recommendation.find("Replace Drive Soon"), std::string::npos);
}

TEST(AIRecommendationTest, HighTemperatureDrive) {
    DriveHealthInfo health;
    health.driveName = "dummyDrive";
    health.temperature = 55.0;
    health.readErrorCount = 0;
    health.writeErrorCount = 0;
    health.overallHealth = 90.0;
    health.smartStatus = "PASSED";
    
    std::string recommendation = AIRecommendation::getRecommendation(health);
    EXPECT_NE(recommendation.find("Backup Data Immediately"), std::string::npos);
}
