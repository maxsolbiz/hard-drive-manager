#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "drive_detection.h"
#include "scanning.h"
#include "drive_health_monitor.h"
#include "drive_repair.h"
#include "partitioning_formatting.h"
#include "drive_cloning.h"
#include "ai_recommendation.h"
#include "logging_reporting.h"

// Helper function to print usage instructions
void printUsage() {
    std::cout << "Usage: hard_drive_manager [--json] --module=<module_name> [arguments]\n"
              << "Available modules:\n"
              << "  detect       - Perform drive detection\n"
              << "  scan         - Perform drive scanning (requires drive name)\n"
              << "  repair       - Perform drive repair (requires drive name and mode: dry_run/live)\n"
              << "  clone        - Perform drive cloning (requires source, destination, and mode: dry_run/live)\n"
              << "  partition    - Perform partitioning & formatting (requires drive name, size, file system, and mode: dry_run/live)\n"
              << "  ai           - Get AI-based recommendation (requires drive name)\n"
              << "  logs         - Retrieve logs\n"
              << "  health       - Get health metrics (requires drive name)\n";
}

int main(int argc, char** argv) {
    // Filter out the optional "--json" flag from arguments.
    std::vector<std::string> args;
    for (int i = 1; i < argc; i++) {
        std::string arg(argv[i]);
        if (arg == "--json") continue;
        args.push_back(arg);
    }
    
    if (args.empty()) {
        printUsage();
        return 1;
    }
    
    // Expect the first argument to be the module flag: --module=<module_name>
    std::string moduleArg = args[0];
    std::string modulePrefix = "--module=";
    if (moduleArg.find(modulePrefix) != 0) {
        printUsage();
        return 1;
    }
    
    std::string module = moduleArg.substr(modulePrefix.size());
    // The remaining arguments (if any) are extra arguments.
    std::vector<std::string> extraArgs(args.begin() + 1, args.end());
    
    // Initialize logging database.
    if (!LoggingReporting::initDB("drive_logs.db")) {
        std::cerr << "Failed to initialize logging database.\n";
        return 1;
    }
    
    if (module == "detect") {
        auto drives = DriveDetection::getDrives();
        std::cout << "Detected Drives:" << std::endl;
        for (const auto &drive : drives) {
            std::cout << drive.toJson() << std::endl;
        }
    }
    else if (module == "scan") {
        if (extraArgs.size() < 1) {
            std::cerr << "Please provide drive name for scanning.\n";
            return 1;
        }
        std::string driveName = extraArgs[0];
        auto drives = DriveDetection::getDrives();
        auto it = std::find_if(drives.begin(), drives.end(), [&](const auto &d) {
            return d.name == driveName;
        });
        if (it == drives.end()) {
            std::cerr << "Drive " << driveName << " not found.\n";
            return 1;
        }
        auto quickResult = Scanning::scanDrive(*it, ScanType::QUICK);
        std::cout << "[Quick Scan] " << it->name << ": " << quickResult.details << "\n";
        auto detailedResult = Scanning::scanDrive(*it, ScanType::DETAILED);
        std::cout << "[Detailed Scan] " << it->name << ": " << detailedResult.details << "\n";
    }
    else if (module == "repair") {
        if (extraArgs.size() < 2) {
            std::cerr << "Please provide drive name and mode (dry_run/live) for repair.\n";
            return 1;
        }
        std::string driveName = extraArgs[0];
        std::string mode = extraArgs[1]; // Expect "dry_run" or "live"
        bool dryRun = (mode == "dry_run");
        auto repairResult = DriveRepair::repairDrive({driveName}, dryRun, false);
        std::cout << "Repair result for " << driveName << ": " << repairResult.details << "\n";
    }
    else if (module == "clone") {
        if (extraArgs.size() < 3) {
            std::cerr << "Please provide source, destination, and mode (dry_run/live) for cloning.\n";
            return 1;
        }
        std::string source = extraArgs[0];
        std::string destination = extraArgs[1];
        std::string mode = extraArgs[2];
        bool dryRun = (mode == "dry_run");
        auto cloneResult = DriveCloning::cloneDrive(source, destination, [](double progress) {}, dryRun, false);
        std::cout << "Cloning result: " << cloneResult.details << "\n";
    }
    else if (module == "partition") {
        if (extraArgs.size() < 4) {
            std::cerr << "Please provide drive name, partition size (GB), file system type, and mode (dry_run/live) for partitioning.\n";
            return 1;
        }
        std::string driveName = extraArgs[0];
        double size = std::stod(extraArgs[1]);
        std::string fileSystem = extraArgs[2];
        std::string mode = extraArgs[3];
        bool dryRun = (mode == "dry_run");
        auto partResult = PartitioningFormatting::createPartition(driveName, PartitionTableType::GPT, size, dryRun, false);
        std::cout << "Partition Creation: " << partResult.details << "\n";
        auto formatResult = PartitioningFormatting::formatPartition(driveName + "1", 
                                                                    (fileSystem == "EXT4" ? FileSystemType::EXT4 : FileSystemType::UNKNOWN), 
                                                                    dryRun, false);
        std::cout << "Partition Formatting: " << formatResult.details << "\n";
    }
    else if (module == "ai") {
        if (extraArgs.size() < 1) {
            std::cerr << "Please provide drive name for AI recommendation.\n";
            return 1;
        }
        std::string driveName = extraArgs[0];
        auto drives = DriveDetection::getDrives();
        auto it = std::find_if(drives.begin(), drives.end(), [&](const auto &d) {
            return d.name == driveName;
        });
        if (it == drives.end()) {
            std::cerr << "Drive " << driveName << " not found.\n";
            return 1;
        }
        auto health = DriveHealthMonitor::getHealthMetrics(*it);
        auto recommendation = AIRecommendation::getRecommendation(health);
        std::cout << "Recommendation for " << driveName << ": " << recommendation << "\n";
    }
    else if (module == "logs") {
        auto logs = LoggingReporting::getLogs();
        for (const auto &logEntry : logs) {
            std::cout << logEntry << "\n";
        }
    }
    else if (module == "health") {
        if (extraArgs.size() < 1) {
            std::cerr << "Please provide drive name for health monitoring.\n";
            return 1;
        }
        std::string driveName = extraArgs[0];
        auto drives = DriveDetection::getDrives();
        auto it = std::find_if(drives.begin(), drives.end(), [&](const auto &d) {
            return d.name == driveName;
        });
        if (it == drives.end()) {
            std::cerr << "Drive " << driveName << " not found.\n";
            return 1;
        }
        auto healthMetrics = DriveHealthMonitor::getHealthMetrics(*it);
        std::string healthJson = healthMetrics.toJson();
        std::cerr << "DEBUG: Health JSON: " << healthJson << std::endl;
        std::cout << healthJson << std::endl;
    }
    
    
    else {
        std::cerr << "Unknown module: " << module << "\n";
        printUsage();
        return 1;
    }
    
    LoggingReporting::closeDB();
    return 0;
}
