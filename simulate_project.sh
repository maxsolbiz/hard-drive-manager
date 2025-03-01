#!/bin/bash
# simulate_project.sh
# This script simulates an end-to-end test of the Disk Management Project.
# It runs the main executable and then queries the SQLite database for log entries.

# Set the path to your main executable and SQLite database.
EXECUTABLE="./build/backend/hard_drive_manager"
DB_PATH="./build/drive_logs.db"

echo "============================================"
echo "Simulating Disk Management Project"
echo "============================================"
echo ""

# Check if the main executable exists.
if [ ! -f "$EXECUTABLE" ]; then
    echo "[ERROR] Main executable not found at $EXECUTABLE. Please build the project first."
    exit 1
fi

# Run the main executable.
echo "[INFO] Running the main executable..."
$EXECUTABLE

# Wait a moment for the operations to complete.
echo ""
echo "[INFO] Main executable finished. Now querying the logs..."
sleep 1

# Check if the SQLite database exists.
if [ ! -f "$DB_PATH" ]; then
    echo "[ERROR] SQLite database not found at $DB_PATH. Ensure that the logging module is initialized."
    exit 1
fi

# Query the SQLite database and display logs in a formatted table.
echo ""
echo "--------------------------------------------"
echo "Logged Operations:"
echo "--------------------------------------------"
sqlite3 "$DB_PATH" <<EOF
.headers on
.mode column
SELECT id, driveName, operationType, details, duration, timestamp FROM logs;
EOF

echo ""
echo "[INFO] Simulation completed."
