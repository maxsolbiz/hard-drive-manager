// backend/src/drive_health_monitor.cpp

#include "drive_health_monitor.h"
#include <sstream>
#include <thread>        // For std::this_thread
#include <chrono>        // For std::chrono
#include <iomanip>
#include "logging_reporting.h"
#include <cstdio>        // For popen, fgets, pclose
#include <cstring>
#include <regex>
#include <iostream>
#include <stdexcept>

// Helper function to fetch SMART data using smartctl command and parse temperature and status.
static std::pair<std::string, double> fetchSmartData(const std::string &devicePath) {
    // Use "smartctl -A" to get all SMART attributes.
    std::string command = "smartctl -A " + devicePath;
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        return { "UNKNOWN", -1.0 };
    }
    char buffer[256];
    std::string output = "";
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        output += buffer;
    }
    pclose(pipe);

    // Determine overall SMART status by checking for keywords "PASSED" or "FAILED"
    std::string status = "UNKNOWN";
    if (output.find("PASSED") != std::string::npos) {
        status = "PASSED";
    } else if (output.find("FAILED") != std::string::npos) {
        status = "FAILED";
    }
    
    // Parse temperature using a regex.
    // Example output line:
    // "194 Temperature_Celsius     0x0032   064   023   000    Old_age   Always       -       36 (Min/Max 22/77)"
    double temperature = -1.0;
    std::regex tempRegex(R"(Temperature_Celsius\s+\S+\s+\S+\s+\S+\s+\S+\s+.*\s+(\d+)\s+\(Min/Max)");
    std::smatch match;
    if (std::regex_search(output, match, tempRegex)) {
        if (match.size() > 1) {
            try {
                temperature = std::stod(match[1].str());
            } catch (const std::invalid_argument &) {
                temperature = -1.0;
            }
        }
    }
    
    return { status, temperature };
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

    // Simulate a delay for real-time measurement.
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // Fetch real SMART data using smartctl.
    auto smartData = fetchSmartData("/dev/" + drive.name);
    info.smartStatus = smartData.first;
    info.temperature = smartData.second;

    // TODO: Enhance parsing to extract actual read error count, write error count, and overall health.
    // For now, we'll use dummy values for these metrics.
    info.readErrorCount = 0;
    info.writeErrorCount = 0;
    info.overallHealth = 98.0;

    // Log the health check operation.
    LoggingReporting::logOperation(drive.name, "HealthCheck", "Real-time health metrics retrieved.", 0.5);

    return info;
}
