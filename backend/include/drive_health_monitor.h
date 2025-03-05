#ifndef DRIVE_HEALTH_MONITOR_H
#define DRIVE_HEALTH_MONITOR_H

#include "drive_info.h"
#include <string>

// Structure to hold drive health metrics.
struct DriveHealthInfo {
    std::string driveName;
    double temperature;            // e.g., Temperature in Celsius
    int readErrorCount;            // e.g., Number of read errors
    int writeErrorCount;           // e.g., Number of write errors
    double overallHealth;          // Composite health metric (can be computed)
    std::string smartStatus;       // e.g., "PASSED" or "FAILED"
    
    // Additional SMART attributes (RAW_VALUE as integer):
    int reallocatedSectorCount;
    int powerOnHours;
    int powerCycleCount;
    int wearLevelingCount;
    int unexpectedPowerLossCount;
    int uncorrectableErrorCount;
    int mediaWearoutIndicator;
    int totalLBAsWritten;
    int totalLBAsRead;
    int nandWrites1GiB;

    std::string toJson() const;
};

class DriveHealthMonitor {
public:
    static DriveHealthInfo getHealthMetrics(const DriveInfo &drive);
};

#endif // DRIVE_HEALTH_MONITOR_H
