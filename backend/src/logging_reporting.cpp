#include "logging_reporting.h"
#include <sqlite3.h>
#include <iostream>
#include <sstream>

namespace LoggingReporting {

sqlite3* db = nullptr;

bool initDB(const std::string &dbPath) {
    int rc = sqlite3_open(dbPath.c_str(), &db);
    if (rc != SQLITE_OK) {
        std::cerr << "[ERROR] Can't open database: " << sqlite3_errmsg(db) << "\n";
        return false;
    }
    // Create the logs table if it doesn't exist.
    const char* createTableSQL =
        "CREATE TABLE IF NOT EXISTS logs ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "driveName TEXT, "
        "operationType TEXT, "
        "details TEXT, "
        "duration REAL, "
        "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP"
        ");";
    char *errMsg = nullptr;
    rc = sqlite3_exec(db, createTableSQL, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "[ERROR] SQL error: " << errMsg << "\n";
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

bool logOperation(const std::string &driveName,
                  const std::string &operationType,
                  const std::string &details,
                  double duration) {
    if (!db) return false;
    
    std::string sql = "INSERT INTO logs (driveName, operationType, details, duration) VALUES (?, ?, ?, ?);";
    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "[ERROR] Failed to prepare statement: " << sqlite3_errmsg(db) << "\n";
        return false;
    }
    
    sqlite3_bind_text(stmt, 1, driveName.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, operationType.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, details.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 4, duration);
    
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "[ERROR] Failed to execute statement: " << sqlite3_errmsg(db) << "\n";
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);
    return true;
}

std::vector<std::string> getLogs() {
    std::vector<std::string> logs;
    if (!db) return logs;
    
    const char* sql = "SELECT id, driveName, operationType, details, duration, timestamp FROM logs;";
    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "[ERROR] Failed to prepare select statement: " << sqlite3_errmsg(db) << "\n";
        return logs;
    }
    
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const unsigned char* driveName = sqlite3_column_text(stmt, 1);
        const unsigned char* operationType = sqlite3_column_text(stmt, 2);
        const unsigned char* details = sqlite3_column_text(stmt, 3);
        double duration = sqlite3_column_double(stmt, 4);
        const unsigned char* timestamp = sqlite3_column_text(stmt, 5);
        
        std::ostringstream oss;
        oss << "ID: " << id 
            << ", Drive: " << (driveName ? reinterpret_cast<const char*>(driveName) : "")
            << ", Operation: " << (operationType ? reinterpret_cast<const char*>(operationType) : "")
            << ", Details: " << (details ? reinterpret_cast<const char*>(details) : "")
            << ", Duration: " << duration << " sec"
            << ", Timestamp: " << (timestamp ? reinterpret_cast<const char*>(timestamp) : "");
        logs.push_back(oss.str());
    }
    sqlite3_finalize(stmt);
    return logs;
}

void closeDB() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }
}

} // namespace LoggingReporting
