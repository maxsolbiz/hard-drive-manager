#include <iostream>
#include "drive_detection.h"
#include "scanning.h"
#include "logging_reporting.h"

int main() {
    std::cout << "Hello, Hard Drive Manager!\n";

    // Initialize the logging database (e.g., "drive_logs.db" in the current directory)
    if (!LoggingReporting::initDB("drive_logs.db")) {
        std::cerr << "[ERROR] Failed to initialize logging database.\n";
        return 1;
    }

    auto drives = DriveDetection::getDrives();

    std::cout << "Detected Drives:\n";
    for (const auto &drive : drives) {
        std::cout << "Name: " << drive.name << "\n"
                  << "  Size: " << drive.size << "\n"
                  << "  Model: " << drive.model << "\n"
                  << "  Vendor: " << drive.vendor << "\n"
                  << "  Model Family: " << drive.modelFamily << "\n"
                  << "  Serial: " << drive.serial << "\n";
                  
        // Perform quick scan
        auto quickResult = Scanning::scanDrive(drive, ScanType::QUICK);
        std::cout << "[INFO] Performing quick scan on drive " << drive.name << "\n"
                  << "  Quick Scan: " << quickResult.details << "\n";
        // Log quick scan
        LoggingReporting::logOperation(drive.name, "QuickScan", quickResult.details, quickResult.durationSeconds);
        
        // Perform detailed scan
        auto detailedResult = Scanning::scanDrive(drive, ScanType::DETAILED);
        std::cout << "[INFO] Performing detailed scan on drive " << drive.name << "\n"
                  << "  Detailed Scan: " << detailedResult.details << "\n\n";
        // Log detailed scan
        LoggingReporting::logOperation(drive.name, "DetailedScan", detailedResult.details, detailedResult.durationSeconds);
    }

    // Optionally, retrieve and display logs.
    std::cout << "\nLogged Operations:\n";
    auto logs = LoggingReporting::getLogs();
    for (const auto &logEntry : logs) {
        std::cout << logEntry << "\n";
    }

    LoggingReporting::closeDB();
    return 0;
}
