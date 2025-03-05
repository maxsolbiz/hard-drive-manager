#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <array>
#include <memory>
#include <cstdio>
#include <nlohmann/json.hpp>
#include <algorithm>
#include <stdexcept>

// For convenience
using json = nlohmann::json;

// Execute a shell command and capture its output.
std::string execCommand(const std::string& cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, int(*)(FILE*)> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

// List physical block devices (only disks)
std::vector<std::string> listPhysicalDevices() {
    std::vector<std::string> devices;
    std::string output = execCommand("lsblk -dno NAME,TYPE | grep disk");
    std::istringstream stream(output);
    std::string line;
    while (std::getline(stream, line)) {
        std::istringstream lineStream(line);
        std::string name, type;
        if (lineStream >> name >> type && type == "disk") {
            devices.push_back("/dev/" + name);
        }
    }
    return devices;
}

// Determine drive type (e.g., HDD, SSD, NVMe) using smartctl info.
std::string getDriveType(const std::string& device) {
    std::string cmd = "sudo smartctl -i " + device;
    std::string output = execCommand(cmd);
    if (output.find("NVMe") != std::string::npos) {
        return "NVMe";
    } else if (output.find("Rotation Rate") != std::string::npos) {
        if (output.find("Solid State Device") != std::string::npos) {
            return "SSD";
        } else {
            return "HDD";
        }
    } else {
        return "Unknown";
    }
}

// Struct to hold SMART details for a drive.
struct DriveInfo {
    std::string device;
    std::string driveType;
    std::string modelFamily;
    std::string deviceModel;
    std::string serialNumber;
    std::string firmwareVersion;
    std::string userCapacity;
    std::string sectorSize;
    std::string rotationRate;
    std::string formFactor;
    std::string healthStatus;
    std::string powerOnHours;
    std::string temperature;
    std::string reallocatedSectors;
    std::string pendingSectors;
    std::string uncorrectableSectors;
    std::string totalLBAsWritten;
    std::string totalLBAsRead;
};

// Function to extract SMART details using tokenization.
DriveInfo getSMARTDetails(const std::string& device) {
    DriveInfo driveInfo;
    driveInfo.device = device;
    driveInfo.driveType = getDriveType(device);
    
    // Run smartctl to get full SMART details.
    std::string cmd = "sudo smartctl -a " + device;
    std::string output = execCommand(cmd);
    
    std::istringstream stream(output);
    std::string line;
    while (std::getline(stream, line)) {
        if (line.find("Model Family:") != std::string::npos) {
            driveInfo.modelFamily = line.substr(line.find(":") + 1);
        } else if (line.find("Device Model:") != std::string::npos) {
            driveInfo.deviceModel = line.substr(line.find(":") + 1);
        } else if (line.find("Serial Number:") != std::string::npos) {
            driveInfo.serialNumber = line.substr(line.find(":") + 1);
        } else if (line.find("Firmware Version:") != std::string::npos) {
            driveInfo.firmwareVersion = line.substr(line.find(":") + 1);
        } else if (line.find("User Capacity:") != std::string::npos) {
            driveInfo.userCapacity = line.substr(line.find(":") + 1);
        } else if (line.find("Sector Size:") != std::string::npos) {
            driveInfo.sectorSize = line.substr(line.find(":") + 1);
        } else if (line.find("Rotation Rate:") != std::string::npos) {
            driveInfo.rotationRate = line.substr(line.find(":") + 1);
        } else if (line.find("Form Factor:") != std::string::npos) {
            driveInfo.formFactor = line.substr(line.find(":") + 1);
        } else if (line.find("SMART overall-health self-assessment test result:") != std::string::npos) {
            driveInfo.healthStatus = line.substr(line.find(":") + 1);
        }
        // For attributes from the SMART table, assume RAW_VALUE is in the 10th token.
        else if (line.find("Power_On_Hours") != std::string::npos) {
            std::istringstream ls(line);
            std::string token;
            for (int i = 0; i < 10; ++i) {
                ls >> token;
            }
            driveInfo.powerOnHours = token;
        } else if (line.find("Temperature_Celsius") != std::string::npos) {
            std::istringstream ls(line);
            std::string token;
            for (int i = 0; i < 10; ++i) {
                ls >> token;
            }
            driveInfo.temperature = token;
        } else if (line.find("Reallocated_Sector_Ct") != std::string::npos) {
            std::istringstream ls(line);
            std::string token;
            for (int i = 0; i < 10; ++i) {
                ls >> token;
            }
            driveInfo.reallocatedSectors = token;
        } else if (line.find("Current_Pending_Sector") != std::string::npos) {
            std::istringstream ls(line);
            std::string token;
            for (int i = 0; i < 10; ++i) {
                ls >> token;
            }
            driveInfo.pendingSectors = token;
        } else if (line.find("Uncorrectable_Sector_Ct") != std::string::npos ||
                   line.find("Offline_Uncorrectable") != std::string::npos) {
            std::istringstream ls(line);
            std::string token;
            for (int i = 0; i < 10; ++i) {
                ls >> token;
            }
            driveInfo.uncorrectableSectors = token;
        } else if (line.find("Total_LBAs_Written") != std::string::npos) {
            std::istringstream ls(line);
            std::string token;
            for (int i = 0; i < 10; ++i) {
                ls >> token;
            }
            driveInfo.totalLBAsWritten = token;
        } else if (line.find("Total_LBAs_Read") != std::string::npos) {
            std::istringstream ls(line);
            std::string token;
            for (int i = 0; i < 10; ++i) {
                ls >> token;
            }
            driveInfo.totalLBAsRead = token;
        }
    }
    return driveInfo;
}

// Convert DriveInfo to JSON.
json driveInfoToJson(const DriveInfo& driveInfo) {
    json j;
    j["device"] = driveInfo.device;
    j["drive_type"] = driveInfo.driveType;
    j["model_family"] = driveInfo.modelFamily;
    j["device_model"] = driveInfo.deviceModel;
    j["serial_number"] = driveInfo.serialNumber;
    j["firmware_version"] = driveInfo.firmwareVersion;
    j["user_capacity"] = driveInfo.userCapacity;
    j["sector_size"] = driveInfo.sectorSize;
    j["rotation_rate"] = driveInfo.rotationRate;
    j["form_factor"] = driveInfo.formFactor;
    j["health_status"] = driveInfo.healthStatus;
    j["power_on_hours"] = driveInfo.powerOnHours;
    j["temperature"] = driveInfo.temperature;
    j["reallocated_sectors"] = driveInfo.reallocatedSectors;
    j["pending_sectors"] = driveInfo.pendingSectors;
    j["uncorrectable_sectors"] = driveInfo.uncorrectableSectors;
    j["total_lbas_written"] = driveInfo.totalLBAsWritten;
    j["total_lbas_read"] = driveInfo.totalLBAsRead;
    return j;
}

int main() {
    try {
        std::cout << "Checking health of all attached hard drives...\n";
        std::vector<std::string> devices = listPhysicalDevices();
        if (devices.empty()) {
            std::cerr << "No physical block devices found.\n";
            return 1;
        }
        json outputJson;
        for (const auto& device : devices) {
            DriveInfo driveInfo = getSMARTDetails(device);
            outputJson["drives"].push_back(driveInfoToJson(driveInfo));
        }
        std::cout << outputJson.dump(4) << "\n";
        std::ofstream jsonFile("drive_health.json");
        jsonFile << outputJson.dump(4);
        jsonFile.close();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
