#ifndef LOGGING_REPORTING_H
#define LOGGING_REPORTING_H

#include <string>
#include <vector>

namespace LoggingReporting {

/**
 * @brief Structure representing a single log entry.
 */
struct LogEntry {
    int id;                 ///< Unique log entry ID.
    std::string driveName;  ///< The name of the drive.
    std::string operationType; ///< The type of operation (e.g., "QuickScan", "DetailedScan").
    std::string details;    ///< Detailed message or result of the operation.
    double duration;        ///< Duration of the operation in seconds.
    std::string timestamp;  ///< Timestamp of when the operation was logged.
};

/**
 * @brief Initializes the logging database.
 * 
 * This function opens (or creates) the SQLite database at the specified path and
 * creates the logs table if it doesn't exist.
 * 
 * @param dbPath Path to the SQLite database file.
 * @return true if initialization is successful, false otherwise.
 */
bool initDB(const std::string &dbPath);

/**
 * @brief Logs an operation performed on a drive.
 * 
 * @param driveName The name of the drive.
 * @param operationType The type of operation (e.g., "QuickScan", "DetailedScan").
 * @param details Detailed message or results of the operation.
 * @param duration Duration of the operation in seconds.
 * @return true if the log is successfully recorded, false otherwise.
 */
bool logOperation(const std::string &driveName,
                  const std::string &operationType,
                  const std::string &details,
                  double duration);

/**
 * @brief Retrieves all logged operations from the database as a vector of strings.
 * 
 * @return A vector of strings, each representing a log entry.
 */
std::vector<std::string> getLogs();

/**
 * @brief Retrieves all logged operations from the database as structured log entries.
 * 
 * @return A vector of LogEntry objects.
 */
std::vector<LogEntry> getLogEntries();

/**
 * @brief Closes the database connection.
 */
void closeDB();

} // namespace LoggingReporting

#endif // LOGGING_REPORTING_H
