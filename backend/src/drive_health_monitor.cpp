#include "drive_health_monitor.h"
#include "logging_reporting.h"
#include "drive_info.h"  // Contains the definition of DriveInfo
#include <sstream>
#include <thread>
#include <chrono>
#include <iomanip>
#include <cstdio>
#include <cstring>
#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>

// Helper function to execute a command and return its output as a string.
static std::string execCommand(const std::string &command) {
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    char buffer[256];
    std::string result;
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }
    pclose(pipe);
    return result;
}

// Helper to parse overall SMART status from smartctl output.
static std::string parseOverallStatus(const std::string &output) {
    std::istringstream iss(output);
    std::string line;
    while (std::getline(iss, line)) {
        if (line.find("SMART overall-health self-assessment test result:") != std::string::npos) {
            size_t pos = line.find(":");
            if (pos != std::string::npos) {
                std::string status = line.substr(pos + 1);
                // Trim whitespace.
                status.erase(0, status.find_first_not_of(" \t"));
                status.erase(status.find_last_not_of(" \t\r\n") + 1);
                return status;
            }
        }
    }
    if (output.find("PASSED") != std::string::npos)
        return "PASSED";
    else if (output.find("FAILED") != std::string::npos)
        return "FAILED";
    return "UNKNOWN";
}

// Fixed-token parser: Given a line that contains the attribute name,
// this function tokenizes the line and returns the token at index 'tokenIndex'.
// If the token isn't found, returns an empty string.
static std::string getTokenFromLine(const std::string &line, int tokenIndex) {
    std::istringstream iss(line);
    std::string token;
    for (int i = 0; i <= tokenIndex; i++) {
        if (!(iss >> token))
            return "";
    }
    return token;
}

// Fixed-token helper to parse an integer attribute from smartctl output.
static int parseAttributeFixed(const std::string &output, const std::string &attrName, int tokenIndex = 9) {
    std::istringstream iss(output);
    std::string line;
    while (std::getline(iss, line)) {
        if (line.find(attrName) != std::string::npos) {
            std::string rawValue = getTokenFromLine(line, tokenIndex);
            try {
                return std::stoi(rawValue);
            } catch (const std::invalid_argument &) {
                return -1;
            }
        }
    }
    return -1;
}

// Fixed-token helper to parse a double attribute (e.g., temperature) from smartctl output.
static double parseTemperatureFixed(const std::string &output, const std::string &attrName, int tokenIndex = 9) {
    std::istringstream iss(output);
    std::string line;
    while (std::getline(iss, line)) {
        if (line.find(attrName) != std::string::npos) {
            std::string rawValue = getTokenFromLine(line, tokenIndex);
            try {
                return std::stod(rawValue);
            } catch (const std::invalid_argument &) {
                return -1.0;
            }
        }
    }
    return -1.0;
}

std::string DriveHealthInfo::toJson() const {
    std::ostringstream oss;
    oss << "{"
        << "\"driveName\":\"" << driveName << "\","
        << "\"temperature\":" << std::fixed << std::setprecision(1) << temperature << ","
        << "\"readErrorCount\":" << readErrorCount << ","
        << "\"writeErrorCount\":" << writeErrorCount << ","
        << "\"overallHealth\":" << overallHealth << ","
        << "\"smartStatus\":\"" << smartStatus << "\","
        << "\"reallocatedSectorCount\":" << reallocatedSectorCount << ","
        << "\"powerOnHours\":" << powerOnHours << ","
        << "\"powerCycleCount\":" << powerCycleCount << ","
        << "\"wearLevelingCount\":" << wearLevelingCount << ","
        << "\"unexpectedPowerLossCount\":" << unexpectedPowerLossCount << ","
        << "\"uncorrectableErrorCount\":" << uncorrectableErrorCount << ","
        << "\"mediaWearoutIndicator\":" << mediaWearoutIndicator << ","
        << "\"totalLBAsWritten\":" << totalLBAsWritten << ","
        << "\"totalLBAsRead\":" << totalLBAsRead << ","
        << "\"nandWrites1GiB\":" << nandWrites1GiB
        << "}";
    return oss.str();
}

DriveHealthInfo DriveHealthMonitor::getHealthMetrics(const DriveInfo &drive) {
    DriveHealthInfo info;
    info.driveName = drive.name;

    // Simulate a delay.
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // Execute smartctl to fetch SMART attributes.
    std::string command = "smartctl -A /dev/" + drive.name;
    std::string output;
    try {
        output = execCommand(command);
    } catch (const std::exception &ex) {
        std::cerr << "Error executing smartctl: " << ex.what() << std::endl;
        output = "";
    }

    // Parse overall SMART status.
    info.smartStatus = parseOverallStatus(output);

    // Parse temperature.
    info.temperature = parseTemperatureFixed(output, "Temperature_Celsius", 9);

    // Parse additional SMART attributes.
    info.reallocatedSectorCount = parseAttributeFixed(output, "Reallocated_Sector_Ct", 9);
    info.powerOnHours = parseAttributeFixed(output, "Power_On_Hours", 9);
    info.powerCycleCount = parseAttributeFixed(output, "Power_Cycle_Count", 9);
    info.wearLevelingCount = parseAttributeFixed(output, "Wear_Leveling_Count", 9);
    info.unexpectedPowerLossCount = parseAttributeFixed(output, "Unexpect_Power_Loss_Ct", 9);
    info.uncorrectableErrorCount = parseAttributeFixed(output, "Uncorrectable_Error_Cnt", 9);
    info.mediaWearoutIndicator = parseAttributeFixed(output, "Media_Wearout_Indicator", 9);
    info.totalLBAsWritten = parseAttributeFixed(output, "Total_LBAs_Written", 9);
    info.totalLBAsRead = parseAttributeFixed(output, "Total_LBAs_Read", 9);
    info.nandWrites1GiB = parseAttributeFixed(output, "NAND_Writes_1GiB", 9);

    // For overallHealth, you can compute a composite metric; for now, we use a dummy value.
    info.overallHealth = 98.0;

    // Log the health check.
    LoggingReporting::logOperation(drive.name, "HealthCheck", "Real-time health metrics retrieved.", 0.5);

    return info;
}
