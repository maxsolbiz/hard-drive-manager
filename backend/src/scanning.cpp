#include "scanning.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <future>
#include <sstream>
#include <iomanip>

//---------------------------------------------------------------------
// Simple Logging Helper (for demo purposes; consider using spdlog)
//---------------------------------------------------------------------
static void logMessage(const std::string &level, const std::string &message) {
    std::cerr << "[" << level << "] " << message << std::endl;
}

//---------------------------------------------------------------------
// JSON Output for ScanResult
//---------------------------------------------------------------------
std::string ScanResult::toJson() const {
    std::ostringstream oss;
    oss << "{"
        << "\"driveName\":\"" << driveName << "\","
        << "\"scanType\":\"" << (type == ScanType::QUICK ? "QUICK" : "DETAILED") << "\","
        << "\"success\":" << (success ? "true" : "false") << ","
        << "\"details\":\"" << details << "\","
        << "\"durationSeconds\":" << std::fixed << std::setprecision(2) << durationSeconds
        << "}";
    return oss.str();
}

//---------------------------------------------------------------------
// Synchronous Scan Implementation
//---------------------------------------------------------------------
ScanResult Scanning::scanDrive(const DriveInfo& drive, ScanType type) {
    ScanResult result;
    result.driveName = drive.name;
    result.type = type;
    auto startTime = std::chrono::steady_clock::now();

    if (type == ScanType::QUICK) {
        logMessage("INFO", "Performing quick scan on drive " + drive.name);
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Simulate quick scan delay
        result.success = true;
        result.details = "Quick scan completed successfully.";
    } else { // DETAILED
        logMessage("INFO", "Performing detailed scan on drive " + drive.name);
        std::this_thread::sleep_for(std::chrono::seconds(2)); // Simulate detailed scan delay
        result.success = true;
        result.details = "Detailed scan completed successfully with no errors found.";
    }
    
    auto endTime = std::chrono::steady_clock::now();
    result.durationSeconds = std::chrono::duration<double>(endTime - startTime).count();
    return result;
}

//---------------------------------------------------------------------
// Asynchronous Scan for Multiple Drives
//---------------------------------------------------------------------
std::vector<ScanResult> Scanning::scanAllDrivesAsync(const std::vector<DriveInfo>& drives, ScanType type) {
    std::vector<std::future<ScanResult>> futures;
    for (const auto &drive : drives) {
        futures.push_back(std::async(std::launch::async, Scanning::scanDrive, drive, type));
    }
    std::vector<ScanResult> results;
    for (auto &f : futures) {
        results.push_back(f.get());
    }
    return results;
}
