#include "drive_health_monitor.h"
#include "drive_detection.h"
#include "logging_reporting.h"
#include <iostream>

int main() {
    // Initialize the logging database.
    if (!LoggingReporting::initDB("drive_logs.db")) {
        std::cerr << "Failed to initialize logging database.\n";
        return 1;
    }
    
    // Obtain drive information from the Drive Detection module.
    auto drives = DriveDetection::getDrives();
    if (drives.empty()) {
        std::cerr << "No drives detected.\n";
        LoggingReporting::closeDB();
        return 1;
    }
    
    // For each detected drive, retrieve and print health metrics.
    for (const auto &drive : drives) {
        std::cout << "Health Metrics for drive " << drive.name << ":\n";
        auto health = DriveHealthMonitor::getHealthMetrics(drive);
        std::cout << health.toJson() << "\n\n";
    }
    
    LoggingReporting::closeDB();
    return 0;
}
