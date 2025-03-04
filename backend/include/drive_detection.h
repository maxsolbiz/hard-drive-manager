#ifndef DRIVE_DETECTION_H
#define DRIVE_DETECTION_H

#include <vector>
#include <string>
#include "drive_info.h"  // Use the consolidated DriveInfo definition

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
