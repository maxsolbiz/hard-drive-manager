#include <iostream>
#include "drive_detection.h"
#include "scanning.h"
#include "drive_health_monitor.h"
#include "drive_repair.h"
#include "partitioning_formatting.h"
#include "drive_cloning.h"
#include "ai_recommendation.h"
#include "logging_reporting.h"

int main() {
    // Initialize the logging database.
    if (!LoggingReporting::initDB("drive_logs.db")) {
        std::cerr << "Failed to initialize logging database.\n";
        return 1;
    }
    
    // --- Drive Detection ---
    auto drives = DriveDetection::getDrives();
    if (drives.empty()) {
        std::cerr << "No drives detected.\n";
        LoggingReporting::closeDB();
        return 1;
    }
    
    // Display detected drives.
    std::cout << "Detected Drives:\n";
    for (const auto &drive : drives) {
        std::cout << drive.toJson() << "\n";
    }
    
    // --- Scanning ---
    std::cout << "\nScanning Drives:\n";
    for (const auto &drive : drives) {
        auto quickResult = Scanning::scanDrive(drive, ScanType::QUICK);
        std::cout << "[Quick Scan] " << drive.name << ": " << quickResult.details << "\n";
        auto detailedResult = Scanning::scanDrive(drive, ScanType::DETAILED);
        std::cout << "[Detailed Scan] " << drive.name << ": " << detailedResult.details << "\n";
    }
    
    // --- Health Monitoring ---
    std::cout << "\nHealth Monitoring:\n";
    for (const auto &drive : drives) {
        auto health = DriveHealthMonitor::getHealthMetrics(drive);
        std::cout << "Health Metrics for " << drive.name << ":\n" << health.toJson() << "\n";
    }
    
    // --- Drive Repair (Dry-run for demonstration) ---
    std::cout << "\nDrive Repair (Dry-run):\n";
    for (const auto &drive : drives) {
        auto repairResult = DriveRepair::repairDrive(drive, true, false);
        std::cout << "Repair result for " << drive.name << ": " << repairResult.details << "\n";
    }
    
    // --- Partitioning & Formatting (Dry-run for demonstration) ---
    std::cout << "\nPartitioning & Formatting (Dry-run):\n";
    // For demonstration, use the first drive detected.
    std::string driveName = drives[0].name;
    auto partResult = PartitioningFormatting::createPartition(driveName, PartitionTableType::GPT, 100.0, true, false);
    std::cout << "Partition Creation: " << partResult.details << "\n";
    auto formatResult = PartitioningFormatting::formatPartition(driveName + "1", FileSystemType::EXT4, true, false);
    std::cout << "Partition Formatting: " << formatResult.details << "\n";
    
    // --- Drive Cloning (Dry-run for demonstration) ---
    std::cout << "\nDrive Cloning (Dry-run):\n";
    auto cloneResult = DriveCloning::cloneDrive("dummySource", "dummyDestination", true, false);
    std::cout << "Cloning result: " << cloneResult.details << "\n";
    
    // --- AI-based Recommendation ---
    std::cout << "\nAI-Based Recommendation:\n";
    // For demonstration, use health metrics from the first drive.
    auto health = DriveHealthMonitor::getHealthMetrics(drives[0]);
    auto recommendation = AIRecommendation::getRecommendation(health);
    std::cout << "Recommendation for " << drives[0].name << ": " << recommendation << "\n";
    
    // --- Logging & Reporting ---
    std::cout << "\nLogged Operations:\n";
    auto logs = LoggingReporting::getLogs();
    for (const auto &logEntry : logs) {
        std::cout << logEntry << "\n";
    }
    
    LoggingReporting::closeDB();
    return 0;
}
