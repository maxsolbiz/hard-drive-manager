#include "partitioning_formatting.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <sstream>
#include "logging_reporting.h"

// Helper function: Convert PartitionTableType to string.
static std::string partitionTableTypeToString(PartitionTableType type) {
    switch(type) {
        case PartitionTableType::GPT: return "GPT";
        case PartitionTableType::MBR: return "MBR";
        default: return "UNKNOWN";
    }
}

// Helper function: Convert FileSystemType to string.
static std::string fileSystemTypeToString(FileSystemType fs) {
    switch(fs) {
        case FileSystemType::FAT32: return "FAT32";
        case FileSystemType::NTFS: return "NTFS";
        case FileSystemType::EXT4: return "EXT4";
        default: return "UNKNOWN";
    }
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

// Simulated rollback function. In case of error, this function simulates a rollback.
static bool rollbackOperation(const std::string &operation) {
    std::cout << "[WARNING] Rolling back operation: " << operation << "\n";
    // Simulate rollback delay.
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return true;
}

PartitionResult PartitioningFormatting::createPartition(const std::string &driveName,
                                                          PartitionTableType tableType,
                                                          double partitionSize,
                                                          bool dryRun,
                                                          bool interactive) {
    PartitionResult result;
    result.rollbackExecuted = false;
    std::ostringstream msg;
    msg << "Creating " << partitionTableTypeToString(tableType)
        << " partition of " << partitionSize << " GB on drive " << driveName << ".";
    std::string operation = msg.str();

    // Interactive confirmation.
    if (!confirmOperation(operation, interactive)) {
        result.success = false;
        result.details = "Operation cancelled by user.";
        return result;
    }
    
    if (dryRun) {
        std::cout << "[INFO] Dry-run mode: " << operation << " simulated.\n";
        result.success = true;
        result.details = "Dry-run: Partition creation simulated successfully.";
        LoggingReporting::logOperation(driveName, "CreatePartitionDryRun", result.details, 0);
        return result;
    }
    
    std::cout << "[INFO] " << operation << "\n";
    std::this_thread::sleep_for(std::chrono::seconds(2)); // Simulate operation delay
    
    // Simulate operation (set simulatedError to true to test rollback).
    bool simulatedError = false;
    if (simulatedError) {
        rollbackOperation("CreatePartition");
        result.success = false;
        result.details = "Error occurred. Operation rolled back.";
        result.rollbackExecuted = true;
    } else {
        result.success = true;
        result.details = "Partition created successfully.";
    }
    
    LoggingReporting::logOperation(driveName, "CreatePartition", result.details, 2.0);
    return result;
}

PartitionResult PartitioningFormatting::formatPartition(const std::string &partitionName,
                                                          FileSystemType fsType,
                                                          bool dryRun,
                                                          bool interactive) {
    PartitionResult result;
    result.rollbackExecuted = false;
    std::ostringstream msg;
    msg << "Formatting partition " << partitionName << " as " << fileSystemTypeToString(fsType) << ".";
    std::string operation = msg.str();
    
    // Interactive confirmation.
    if (!confirmOperation(operation, interactive)) {
        result.success = false;
        result.details = "Operation cancelled by user.";
        return result;
    }
    
    if (dryRun) {
        std::cout << "[INFO] Dry-run mode: " << operation << " simulated.\n";
        result.success = true;
        result.details = "Dry-run: Partition formatting simulated successfully.";
        LoggingReporting::logOperation(partitionName, "FormatPartitionDryRun", result.details, 0);
        return result;
    }
    
    std::cout << "[INFO] " << operation << "\n";
    std::this_thread::sleep_for(std::chrono::seconds(3)); // Simulate operation delay

    bool simulatedError = false;
    if (simulatedError) {
        rollbackOperation("FormatPartition");
        result.success = false;
        result.details = "Error occurred. Operation rolled back.";
        result.rollbackExecuted = true;
    } else {
        result.success = true;
        result.details = "Partition formatted successfully.";
    }
    
    LoggingReporting::logOperation(partitionName, "FormatPartition", result.details, 3.0);
    return result;
}
