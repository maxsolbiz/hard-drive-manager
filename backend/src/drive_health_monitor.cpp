#include "drive_health_monitor.h"
#include <sstream>
#include <thread>
#include <chrono>
#include <iomanip>
#include "logging_reporting.h"

// Helper function: Simulate executing a system command to fetch SMART data.
static std::string simulateSmartData(const std::string &devicePath) {
    // In a real system, you might use popen() to run: smartctl -H <devicePath>
    // For simulation, we return a dummy SMART status.
    return "PASSED";
}

std::string DriveHealthInfo::toJson() const {
    std::ostringstream oss;
    oss << "{"
        << "\"driveName\":\"" << driveName << "\","
        << "\"temperature\":" << std::fixed << std::setprecision(1) << temperature << ","
        << "\"readErrorCount\":" << readErrorCount << ","
        << "\"writeErrorCount\":" << writeErrorCount << ","
        << "\"overallHealth\":" << overallHealth << ","
        << "\"smartStatus\":\"" << smartStatus << "\""
        << "}";
    return oss.str();
}

DriveHealthInfo DriveHealthMonitor::getHealthMetrics(const DriveInfo &drive) {
    DriveHealthInfo info;
    info.driveName = drive.name;

    // Simulate a delay in fetching SMART data.
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // In a real implementation, you would run a command like:
    // smartctl -H /dev/sda
    // and parse its output. Here we simulate:
    info.smartStatus = simulateSmartData("/dev/" + drive.name);

    // Simulated metrics:
    info.temperature = 35.5;  // Celsius
    info.readErrorCount = 0;  // Assume no errors
    info.writeErrorCount = 0; // Assume no errors
    info.overallHealth = 98.0; // Health rating on a scale of 0-100

    // Log the health check operation.
    LoggingReporting::logOperation(drive.name, "HealthCheck", "Health metrics retrieved successfully.", 0.5);

    return info;
}
