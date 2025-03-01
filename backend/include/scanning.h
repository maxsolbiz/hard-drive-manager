#ifndef SCANNING_H
#define SCANNING_H

#include <string>
#include <vector>
#include "drive_detection.h"

/**
 * @brief Enumeration to differentiate scan types.
 */
enum class ScanType {
    QUICK,    ///< Quick scan: basic check for drive health/status.
    DETAILED  ///< Detailed scan: in-depth analysis.
};

/**
 * @brief Structure to hold the result of a scan operation.
 */
struct ScanResult {
    std::string driveName;    ///< The name of the drive that was scanned.
    ScanType type;            ///< The type of scan performed.
    bool success;             ///< Indicates if the scan was successful.
    std::string details;      ///< Detailed results or error messages.
    double durationSeconds;   ///< Duration of the scan in seconds.

    /**
     * @brief Convert the scan result to a JSON-formatted string.
     * @return JSON string representing this scan result.
     */
    std::string toJson() const;
};

/**
 * @brief The Scanning class encapsulates methods for performing scans on drives.
 *
 * Future enhancements can include progress callbacks, cancellation, and pluggable scan strategies.
 */
class Scanning {
public:
    /**
     * @brief Performs a scan on a single drive.
     * @param drive The drive information.
     * @param type The type of scan (quick or detailed).
     * @return The result of the scan.
     */
    static ScanResult scanDrive(const DriveInfo& drive, ScanType type);

    /**
     * @brief Performs scans on multiple drives concurrently.
     * @param drives A vector of drive information.
     * @param type The type of scan to perform.
     * @return A vector of scan results.
     */
    static std::vector<ScanResult> scanAllDrivesAsync(const std::vector<DriveInfo>& drives, ScanType type);
};

#endif // SCANNING_H
