#ifndef DRIVE_REPAIR_H
#define DRIVE_REPAIR_H

#include <string>
#include "drive_detection.h"

/**
 * @brief Structure representing the result of a drive repair operation.
 */
struct DriveRepairResult {
    bool success;           ///< True if the repair operation was successful.
    std::string details;    ///< Detailed message about the repair outcome.
    double durationSeconds; ///< Duration of the repair operation in seconds.
    bool rollbackExecuted;  ///< Indicates if a rollback was executed on failure.
};

/**
 * @brief The DriveRepair class encapsulates functions for repairing drive errors.
 *
 * This module now integrates with the system repair utility (fsck) to perform
 * actual repairs, captures error output, checks for sufficient privileges, and
 * simulates health monitoring integration.
 */
class DriveRepair {
public:
    /**
     * @brief Repairs a drive by fixing minor errors or corrupted sectors.
     *
     * @param drive The drive information obtained from the Drive Detection module.
     * @param dryRun If true, simulates the repair without making changes.
     * @param interactive If true, prompts the user for confirmation before proceeding.
     * @return A DriveRepairResult structure with the outcome of the repair operation.
     */
    static DriveRepairResult repairDrive(const DriveInfo &drive, bool dryRun = false, bool interactive = false);
};

#endif // DRIVE_REPAIR_H
