#ifndef DRIVE_DETECTION_H
#define DRIVE_DETECTION_H

#include <vector>
#include <string>

/**
 * @brief Structure to hold detailed drive information.
 */
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
    std::string toJson() const;
};

/**
 * @brief DriveDetection class encapsulates methods to detect and retrieve drive information.
 */
class DriveDetection {
public:
    /**
     * @brief Returns a vector of connected drives with detailed information.
     * @return Vector of DriveInfo structures.
     */
    static std::vector<DriveInfo> getDrives();
};

#endif // DRIVE_DETECTION_H
