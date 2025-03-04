// backend/src/drive_health_monitor.cpp

#include "drive_health_monitor.h"
#include <sstream>
#include <thread>        // For std::this_thread
#include <chrono>        // For std::chrono
#include <iomanip>
#include "logging_reporting.h"
#include <cstdio>        // For popen, fgets, pclose
#include <cstring>
#include <utility>
#include <iostream>
#include <stdexcept>

// Helper function to fetch SMART data using smartctl command and parse temperature and overall status.
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

    // Determine overall health status (by checking for "PASSED" or "FAILED")
    std::string status = "UNKNOWN";
    if (output.find("PASSED") != std::string::npos) {
        status = "PASSED";
    } else if (output.find("FAILED") != std::string::npos) {
        status = "FAILED";
    }

    // Parse temperature: look for "Temperature_Celsius" in the output.
    double temperature = -1.0;
    size_t pos = output.find("Temperature_Celsius");
    if (pos != std::string::npos) {
        // Extract the line containing the temperature attribute.
        size_t endLine = output.find("\n", pos);
        std::string line = output.substr(pos, endLine - pos);
        // Tokenize the line by whitespace.
        std::istringstream iss(line);
        std::string token;
        int tokenIndex = 0;
        double tempValue = -1.0;
        while (iss >> token) {
            tokenIndex++;
            // For example, assume the temperature is in the 6th token.
            if (tokenIndex == 6) {
                try {
                    tempValue = std::stod(token);
                } catch (std::invalid_argument &) {
                    tempValue = -1.0;
                }
                break;
            }
        }
        temperature = tempValue;
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

    // Optional: simulate delay for real-time measurement.
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // Fetch real SMART data using smartctl.
    auto smartData = fetchSmartData("/dev/" + drive.name);
    info.smartStatus = smartData.first;
    info.temperature = smartData.second;

    // TODO: Enhance parsing to extract actual readErrorCount, writeErrorCount, and overallHealth.
    // For now, we set them as dummy values if not parsed.
    info.readErrorCount = 0;
    info.writeErrorCount = 0;
    info.overallHealth = 98.0;

    // Log the health check operation.
    LoggingReporting::logOperation(drive.name, "HealthCheck", "Real-time health metrics retrieved.", 0.5);

    return info;
}
