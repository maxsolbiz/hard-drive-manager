#ifndef DRIVE_CLONING_H
#define DRIVE_CLONING_H

#include <string>

/**
 * @brief Structure representing the result of a drive cloning operation.
 */
struct DriveCloningResult {
    bool success;           ///< True if cloning was successful.
    std::string details;    ///< Detailed outcome message.
    double durationSeconds; ///< Time taken for the cloning operation.
};

/**
 * @brief The DriveCloning class encapsulates functions to clone drives or partitions.
 *
 * In a production environment, this module would call system utilities (e.g., dd or Clonezilla)
 * to perform bit-for-bit cloning. Here, we simulate the cloning process.
 */
class DriveCloning {
public:
    /**
     * @brief Clones a drive or partition from a source to a destination.
     *
     * @param sourceDrive The name or identifier of the source drive (e.g., "sda").
     * @param destinationDrive The name or identifier of the destination drive (e.g., "sdb").
     * @param dryRun If true, simulates the cloning process without making changes.
     * @param interactive If true, prompts the user for confirmation before proceeding.
     * @return A DriveCloningResult structure with the outcome of the cloning operation.
     */
    static DriveCloningResult cloneDrive(const std::string &sourceDrive,
                                           const std::string &destinationDrive,
                                           bool dryRun = false,
                                           bool interactive = false);
};

#endif // DRIVE_CLONING_H
