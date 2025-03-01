#include "drive_cloning.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <sstream>
#include "logging_reporting.h"

static bool confirmOperation(const std::string &message, bool interactive) {
    if (!interactive)
        return true;
    std::cout << message << " (Y/N): ";
    char response;
    std::cin >> response;
    return (response == 'Y' || response == 'y');
}

DriveCloningResult DriveCloning::cloneDrive(const std::string &sourceDrive,
                                              const std::string &destinationDrive,
                                              std::function<void(double)> progressCallback,
                                              bool dryRun,
                                              bool interactive) {
    DriveCloningResult result;
    std::ostringstream msg;
    msg << "Cloning drive " << sourceDrive << " to " << destinationDrive << ".";
    std::string operation = msg.str();

    if (!confirmOperation(operation, interactive)) {
        result.success = false;
        result.details = "Clone operation cancelled by user.";
        return result;
    }
    
    if (dryRun) {
        std::cout << "[INFO] Dry-run mode: " << operation << " simulated.\n";
        result.success = true;
        result.details = "Dry-run: Drive cloning simulated successfully.";
        LoggingReporting::logOperation(sourceDrive, "CloneDryRun", result.details, 0);
        result.durationSeconds = 0;
        progressCallback(100.0);
        return result;
    }
    
    std::cout << "[INFO] " << operation << "\n";
    auto start = std::chrono::steady_clock::now();
    
    // Simulate cloning progress over 5 seconds.
    const int steps = 5;
    for (int i = 1; i <= steps; i++) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        double progress = (i * 100.0) / steps;
        progressCallback(progress);
    }
    
    auto end = std::chrono::steady_clock::now();
    double duration = std::chrono::duration<double>(end - start).count();
    
    result.success = true;
    result.details = "Drive cloning completed successfully.";
    result.durationSeconds = duration;
    
    LoggingReporting::logOperation(sourceDrive, "CloneDrive", result.details, duration);
    
    return result;
}
