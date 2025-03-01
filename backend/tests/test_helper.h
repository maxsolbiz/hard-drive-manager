#ifndef TEST_HELPER_H
#define TEST_HELPER_H

#include "drive_detection.h"

/**
 * @brief Creates a dummy DriveInfo structure for testing purposes.
 * @param name The name to assign to the dummy drive.
 * @return A dummy DriveInfo object.
 */
inline DriveInfo createDummyDrive(const std::string &name) {
    DriveInfo drive;
    drive.name = name;
    drive.size = "500 GB";
    drive.model = "DUMMYMODEL";
    drive.vendor = "DUMMYVENDOR";
    drive.modelFamily = "DUMMYFAMILY";
    drive.serial = "DUMMYSERIAL";
    return drive;
}

#endif // TEST_HELPER_H
