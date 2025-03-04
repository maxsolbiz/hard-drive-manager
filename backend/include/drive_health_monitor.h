// drive_health_monitor.h
#ifndef DRIVE_HEALTH_MONITOR_H
#define DRIVE_HEALTH_MONITOR_H

#include <string>
#include "drive_info.h"  // If DriveInfo is defined here

struct DriveHealthInfo {
    std::string driveName;
    double temperature;
    int readErrorCount;
    int writeErrorCount;
    double overallHealth;
    std::string smartStatus;
    
    std::string toJson() const;
};

class DriveHealthMonitor {
public:
    static DriveHealthInfo getHealthMetrics(const DriveInfo &drive);
};

#endif
