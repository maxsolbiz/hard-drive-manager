#ifndef DRIVE_CLONING_H
#define DRIVE_CLONING_H

#include <string>
#include <functional>

/**
 * @brief Structure representing the result of a drive cloning operation.
 */
struct DriveCloningResult {
    bool success;
    std::string details;
    double durationSeconds;
};

/**
 * @brief The DriveCloning class encapsulates functions to clone drives or partitions.
 */
class DriveCloning {
public:
    /**
     * @brief Clones a drive or partition from source to destination.
     * 
     * @param sourceDrive Source drive identifier.
     * @param destinationDrive Destination drive identifier.
     * @param progressCallback Callback to report progress (0.0 to 100.0).
     * @param dryRun If true, simulates the operation.
     * @param interactive If true, prompts user for confirmation.
     * @return DriveCloningResult with outcome.
     */
    static DriveCloningResult cloneDrive(const std::string &sourceDrive,
                                           const std::string &destinationDrive,
                                           std::function<void(double)> progressCallback,
                                           bool dryRun = false,
                                           bool interactive = false);
};

#endif // DRIVE_CLONING_H
