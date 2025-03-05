#include "drive_detection.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <cstdio>
#include <map>
#include <vector>
#include <utility>
#include <ctime>

//=========================================================================
// Logging Helper
//-------------------------------------------------------------------------
/**
 * @brief Logs a message with a given log level.
 * @param level The severity level (e.g., INFO, WARNING, ERROR).
 * @param message The message to log.
 */
static void logMessage(const std::string &level, const std::string &message) {
    std::cerr << "[" << level << "] " << message << std::endl;
}

//=========================================================================
// JSON Output for DriveInfo
//-------------------------------------------------------------------------
/*std::string DriveInfo::toJson() const {
    std::ostringstream oss;
    oss << "{"
        << "\"name\":\"" << name << "\","
        << "\"size\":\"" << size << "\","
        << "\"model\":\"" << model << "\","
        << "\"vendor\":\"" << vendor << "\","
        << "\"modelFamily\":\"" << modelFamily << "\","
        << "\"serial\":\"" << serial << "\""
        << "}";
    return oss.str();
}*/

//=========================================================================
// Helper Functions
//-------------------------------------------------------------------------

/**
 * @brief Reads the first line from a file and removes all whitespace.
 * @param filePath The path to the file.
 * @return The first line as a trimmed string, or an empty string if the file cannot be read.
 */
static std::string readFirstLine(const std::string &filePath) {
    std::ifstream file(filePath);
    std::string line;
    if (file.good() && std::getline(file, line)) {
        // Remove whitespace characters
        line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
        return line;
    }
    return "";
}

/**
 * @brief Formats a block count (as a string) to a human‐readable size in GB/TB.
 * @param blockCountStr The block count in string form (assumed 1024-byte blocks).
 * @return Formatted size string (e.g., "465.76 GB").
 */
static std::string formatSize(const std::string &blockCountStr) {
    long blocks = std::stol(blockCountStr);
    double bytes = blocks * 1024.0; // each block is 1024 bytes
    double sizeGB = bytes / (1024.0 * 1024.0 * 1024.0);
    
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    if (sizeGB < 1024) {
        oss << sizeGB << " GB";
    } else {
        double sizeTB = sizeGB / 1024.0;
        oss << sizeTB << " TB";
    }
    return oss.str();
}

/**
 * @brief Loads vendor rules from a configuration file, if available.
 *
 * The configuration file (if present) should be named "vendor_rules.cfg" and located
 * in the working directory. Each line should contain a prefix and a vendor name separated by whitespace.
 * For example:
 *   SAMSUNG SAMSUNG
 *   HGST HGST
 *   ST SEAGATE
 *
 * @return A vector of pairs where first is the model prefix and second is the vendor.
 */
static std::vector<std::pair<std::string, std::string>> loadVendorRules() {
    std::vector<std::pair<std::string, std::string>> rules;
    std::ifstream cfg("vendor_rules.cfg");
    if (!cfg.is_open()) {
        // Default rules if config file is not found.
        rules.push_back({"SAMSUNG", "SAMSUNG"});
        rules.push_back({"HGST", "HGST"});
        rules.push_back({"ST", "SEAGATE"});
        rules.push_back({"WDC", "WESTERN DIGITAL"});
        rules.push_back({"KINGSTON", "KINGSTON"});
        rules.push_back({"TOSHIBA", "TOSHIBA"});
        rules.push_back({"Hitachi", "HITACHI"});
        return rules;
    }
    std::string line;
    while (std::getline(cfg, line)) {
        std::istringstream iss(line);
        std::string prefix, vendor;
        if (iss >> prefix >> vendor) {
            rules.push_back({prefix, vendor});
        }
    }
    return rules;
}

/**
 * @brief Returns a reference to the cached vendor rules.
 *
 * This caches the vendor rules on the first call to avoid reloading the configuration file.
 *
 * @return Reference to a vector of vendor rule pairs.
 */
static const std::vector<std::pair<std::string, std::string>> &getVendorRules() {
    static std::vector<std::pair<std::string, std::string>> rules = loadVendorRules();
    return rules;
}

/**
 * @brief Processes the raw model string to separate the vendor and model family.
 *
 * The function checks the beginning of the model string against known prefixes
 * (loaded from the configuration) and extracts the vendor and the remaining string as the model family.
 *
 * @param rawModel The raw model string.
 * @param vendorOut Output parameter to hold the vendor.
 * @param modelFamilyOut Output parameter to hold the model family.
 */
static void processModel(const std::string &rawModel, std::string &vendorOut, std::string &modelFamilyOut) {
    std::string model = rawModel;
    // Remove all whitespace
    model.erase(std::remove_if(model.begin(), model.end(), ::isspace), model.end());
    
    const auto &rules = getVendorRules();
    bool matched = false;
    for (const auto &rule : rules) {
        if (model.find(rule.first) == 0) {
            vendorOut = rule.second;
            modelFamilyOut = model.substr(rule.first.size());
            matched = true;
            break;
        }
    }
    if (!matched) {
        vendorOut = "";
        modelFamilyOut = model;
    }
}

/**
 * @brief Uses the lsblk command to fetch serial numbers for drives.
 *
 * The output is cached for 60 seconds to improve performance.
 *
 * @return A map from drive name to its serial number.
 */
static std::map<std::string, std::string> getSerialMap() {
    static std::map<std::string, std::string> serialMap;
    static std::time_t lastFetch = 0;
    const int cacheDuration = 60; // seconds
    std::time_t now = std::time(nullptr);
    if ((now - lastFetch < cacheDuration) && !serialMap.empty()) {
        return serialMap;
    }
    serialMap.clear();
    
    FILE *pipe = popen("lsblk -n -o NAME,SERIAL", "r");
    if (!pipe) {
        logMessage("ERROR", "Failed to run lsblk to get serial numbers");
        return serialMap;
    }
    
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        std::string line(buffer);
        if (!line.empty() && line.back() == '\n')
            line.pop_back();
        
        std::istringstream iss(line);
        std::string name, serial;
        iss >> name >> serial;
        serialMap[name] = serial;
    }
    pclose(pipe);
    lastFetch = now;
    return serialMap;
}

//=========================================================================
// Main Drive Detection Implementation
//-------------------------------------------------------------------------

std::vector<DriveInfo> DriveDetection::getDrives() {
    std::vector<DriveInfo> drives;
    
    // Fetch serial numbers using lsblk (cached)
    std::map<std::string, std::string> serialMap = getSerialMap();
    
    // Open /proc/partitions to read basic drive info.
    std::ifstream partitions("/proc/partitions");
    if (!partitions.is_open()) {
        logMessage("ERROR", "Unable to open /proc/partitions");
        return drives;
    }
    
    std::string line;
    // Skip header lines (first two lines)
    std::getline(partitions, line);
    std::getline(partitions, line);
    
    while (std::getline(partitions, line)) {
        std::istringstream iss(line);
        int major, minor;
        std::string blocks, name;
        if (!(iss >> major >> minor >> blocks >> name)) {
            continue;  // Skip lines that do not match expected format.
        }
        // Skip partition entries by ignoring names that contain numbers.
        if (name.find_first_of("0123456789") != std::string::npos)
            continue;
        
        DriveInfo drive;
        drive.name = name;
        drive.size = formatSize(blocks);
        
        // Construct the base sysfs path for the drive.
        std::string basePath = "/sys/block/" + name + "/device/";
        
        // Read the raw model from sysfs.
        std::string rawModel = readFirstLine(basePath + "model");
        drive.model = rawModel;
        
        // Process the model to extract vendor and model family.
        processModel(rawModel, drive.vendor, drive.modelFamily);
        
        // Attempt to read the serial number from sysfs.
        drive.serial = readFirstLine(basePath + "serial");
        // If sysfs did not provide a serial, check the lsblk output.
        if (drive.serial.empty() && serialMap.find(name) != serialMap.end()) {
            drive.serial = serialMap[name];
        }
        
        drives.push_back(drive);
    }
    
    partitions.close();
    return drives;
}
