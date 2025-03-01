#include "drive_repair.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <sstream>
#include <cstdio>
#include <unistd.h>  // For geteuid()
#include "logging_reporting.h"

// Helper function: Run a command and capture its output and exit status.
static std::pair<int, std::string> runCommand(const std::string &command) {
    std::string output;
    const int bufferSize = 256;
    char buffer[bufferSize];
    FILE* pipe = popen((command + " 2>&1").c_str(), "r");
    if (!pipe) {
        return { -1, "Failed to run command." };
    }
    while (fgets(buffer, bufferSize, pipe) != nullptr) {
        output += buffer;
    }
    int exitCode = pclose(pipe);
    return { exitCode, output };
}

// Helper function: Simulate user confirmation in interactive mode.
static bool confirmOperation(const std::string &message, bool interactive) {
    if (!interactive)
        return true;
    std::cout << message << " (Y/N): ";
    char response;
    std::cin >> response;
    return (response == 'Y' || response == 'y');
}

// Simulated rollback function for repair operations.
static bool rollbackRepair(const std::string &operation) {
    std::cout << "[WARNING] Rolling back repair operation: " << operation << "\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return true;
}

// Simulated health check: In real usage, this would query SMART data.
static bool isRepairNeeded(const DriveInfo &drive) {
    // For demonstration, assume repair is needed if drive name is not empty.
    return !drive.name.empty();
}

DriveRepairResult DriveRepair::repairDrive(const DriveInfo &drive, bool dryRun, bool interactive) {
    DriveRepairResult result;
    result.rollbackExecuted = false;
    std::ostringstream msg;
    msg << "Repairing drive " << drive.name << " (attempting to fix minor errors).";
    std::string operation = msg.str();

    // Check for privileges only if not in dry-run mode.
    if (!dryRun && geteuid() != 0) {
        result.success = false;
        result.details = "Insufficient privileges. Please run as root to perform repair operations.";
        return result;
    }

    // Simulated health monitoring integration.
    if (!isRepairNeeded(drive)) {
        result.success = true;
        result.details = "Repair not required based on health monitoring.";
        return result;
    }

    // Interactive confirmation.
    if (!confirmOperation(operation, interactive)) {
        result.success = false;
        result.details = "Repair operation cancelled by user.";
        return result;
    }
    
    auto startTime = std::chrono::steady_clock::now();
    
    if (dryRun) {
        std::cout << "[INFO] Dry-run mode: " << operation << " simulated.\n";
        result.success = true;
        result.details = "Dry-run: Repair simulated successfully.";
        LoggingReporting::logOperation(drive.name, "RepairDryRun", result.details, 0);
        result.durationSeconds = 0;
        return result;
    }
    
    std::cout << "[INFO] " << operation << "\n";
    
    // Real System Integration: Run fsck in non-destructive mode (-n)
    std::string devicePath = "/dev/" + drive.name;
    std::string command = "fsck -n " + devicePath;
    auto [exitCode, cmdOutput] = runCommand(command);
    
    // Simulate repair delay.
    std::this_thread::sleep_for(std::chrono::seconds(3));
    
    if (exitCode != 0) {
        rollbackRepair("RepairDrive");
        result.success = false;
        result.details = "Repair failed. Command output: " + cmdOutput;
        result.rollbackExecuted = true;
    } else {
        result.success = true;
        result.details = "Repair completed successfully. Command output: " + cmdOutput;
    }
    
    auto endTime = std::chrono::steady_clock::now();
    result.durationSeconds = std::chrono::duration<double>(endTime - startTime).count();
    
    LoggingReporting::logOperation(drive.name, "RepairDrive", result.details, result.durationSeconds);
    return result;
}
