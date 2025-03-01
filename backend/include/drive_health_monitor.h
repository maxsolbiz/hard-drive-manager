#ifndef DRIVE_HEALTH_MONITOR_H
#define DRIVE_HEALTH_MONITOR_H

#include <string>
#include "drive_detection.h"

/**
 * @brief Structure representing drive health metrics.
 */
struct DriveHealthInfo {
    std::string driveName;      ///< Name of the drive (e.g., "sda").
    double temperature;         ///< Drive temperature in Celsius.
    int readErrorCount;         ///< Count of read errors.
    int writeErrorCount;        ///< Count of write errors.
    double overallHealth;       ///< Overall health rating (0.0 to 100.0).
    std::string smartStatus;    ///< Summary of SMART status (e.g., "PASSED" or "FAILED").

    /**
     * @brief Converts the health metrics into a JSON-formatted string.
     * @return JSON string representing the drive health data.
     */
    std::string toJson() const;
};

/**
 * @brief The DriveHealthMonitor class encapsulates functions for monitoring drive health.
 *
 * In a production system, this module would invoke tools such as smartctl to retrieve SMART data.
 * Here, it simulates the process and returns dummy health metrics.
 */
class DriveHealthMonitor {
public:
    /**
     * @brief Retrieves health metrics for the specified drive.
     * 
     * @param drive The drive information from the Drive Detection module.
     * @return A DriveHealthInfo structure with health metrics.
     */
    static DriveHealthInfo getHealthMetrics(const DriveInfo &drive);
};

#endif // DRIVE_HEALTH_MONITOR_H
