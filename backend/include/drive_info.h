// backend/include/drive_info.h
#ifndef DRIVE_INFO_H
#define DRIVE_INFO_H

#include <string>
#include <sstream>

struct DriveInfo {
    std::string name;        ///< e.g., "sda", "sdb"
    std::string size;        ///< Formatted size (e.g., "465.76 GB" or "1.23 TB")
    std::string model;       ///< Raw model string read from sysfs
    std::string vendor;      ///< Determined vendor (e.g., "SAMSUNG", "HGST", "SEAGATE")
    std::string modelFamily; ///< Model family extracted from model string
    std::string serial;      ///< Drive serial number (if available)

    /**
     * @brief Convert drive information to a JSON-formatted string.
     * @return JSON string representing this drive.
     */

    std::string toJson() const {
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
    }
};

#endif // DRIVE_INFO_H
