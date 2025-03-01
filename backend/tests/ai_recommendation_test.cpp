#include "ai_recommendation.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <stdexcept>
#include <algorithm>
#include <cctype>
#include <unistd.h>   // For mkstemp, close
#include <cstdlib>    // For std::remove

// Helper function: Trim whitespace from both ends of a string.
static std::string trim(const std::string &str) {
    std::string s = str;
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
    return s;
}

// Helper function: Execute a command and capture its output.
static std::string runPythonScript(const std::string &command) {
    std::string output;
    const int bufferSize = 256;
    char buffer[bufferSize];
    FILE* pipe = popen((command + " 2>&1").c_str(), "r");
    if (!pipe) {
        throw std::runtime_error("Failed to run Python script.");
    }
    while (fgets(buffer, bufferSize, pipe) != nullptr) {
        output += buffer;
    }
    pclose(pipe);
    return trim(output);
}

std::string AIRecommendation::getRecommendation(const DriveHealthInfo &healthInfo) {
    // Construct a JSON string from the health metrics.
    std::ostringstream oss;
    oss << "{"
        << "\"driveName\":\"" << healthInfo.driveName << "\","
        << "\"temperature\":" << healthInfo.temperature << ","
        << "\"readErrorCount\":" << healthInfo.readErrorCount << ","
        << "\"writeErrorCount\":" << healthInfo.writeErrorCount << ","
        << "\"overallHealth\":" << healthInfo.overallHealth << ","
        << "\"smartStatus\":\"" << healthInfo.smartStatus << "\""
        << "}";
    std::string healthJson = oss.str();
    
    // Create a temporary file using mkstemp.
    char tmpFileName[] = "/tmp/healthXXXXXX";
    int fd = mkstemp(tmpFileName);
    if(fd == -1) {
        return "Error: Unable to create temporary file.";
    }
    // Write healthJson to this file.
    FILE* tmpFile = fdopen(fd, "w");
    if (!tmpFile) {
        close(fd);
        return "Error: Unable to open temporary file for writing.";
    }
    fputs(healthJson.c_str(), tmpFile);
    fclose(tmpFile);

    // Build the command to call the Python script.
    // Assuming ai_model.py is in the project root, one directory above the build directory.
    std::string command = "python3 ../ai_model.py " + std::string(tmpFileName);
    
    std::string recommendation;
    try {
        recommendation = runPythonScript(command);
    } catch (std::exception &e) {
        recommendation = "Error obtaining recommendation: " + std::string(e.what());
    }
    
    // Remove the temporary file.
    std::remove(tmpFileName);
    
    return recommendation;
}
