#ifndef PARTITIONING_FORMATTING_H
#define PARTITIONING_FORMATTING_H

#include <string>

/**
 * @brief Enumeration for partition table type.
 */
enum class PartitionTableType {
    GPT,
    MBR
};

/**
 * @brief Enumeration for file system types.
 */
enum class FileSystemType {
    FAT32,
    NTFS,
    EXT4,
    UNKNOWN
};

/**
 * @brief Structure representing the result of a partitioning or formatting operation.
 */
struct PartitionResult {
    bool success;           ///< Operation success flag.
    std::string details;    ///< Detailed message or error.
    bool rollbackExecuted;  ///< True if a rollback was executed due to failure.
};

/**
 * @brief The PartitioningFormatting class encapsulates functions for managing partitions
 * and formatting drives.
 *
 * This module supports:
 * - Dry-run mode: Simulates the operation without making changes.
 * - Interactive mode: Prompts the user for confirmation before execution.
 * - Rollback: Simulated rollback mechanism on error.
 */
class PartitioningFormatting {
public:
    /**
     * @brief Creates a partition on a specified drive.
     *
     * @param driveName Name of the drive (e.g., "sda").
     * @param tableType The partition table type (GPT or MBR).
     * @param partitionSize Size of the partition to create (in GB).
     * @param dryRun If true, simulates the operation without making changes.
     * @param interactive If true, prompts the user for confirmation.
     * @return A PartitionResult indicating success or failure.
     */
    static PartitionResult createPartition(const std::string &driveName,
                                             PartitionTableType tableType,
                                             double partitionSize,
                                             bool dryRun = false,
                                             bool interactive = false);

    /**
     * @brief Formats a specified partition with a given file system.
     *
     * @param partitionName The name of the partition (e.g., "sda1").
     * @param fsType The file system type (FAT32, NTFS, EXT4).
     * @param dryRun If true, simulates the operation without making changes.
     * @param interactive If true, prompts the user for confirmation.
     * @return A PartitionResult indicating success or failure.
     */
    static PartitionResult formatPartition(const std::string &partitionName,
                                             FileSystemType fsType,
                                             bool dryRun = false,
                                             bool interactive = false);
};

#endif // PARTITIONING_FORMATTING_H
