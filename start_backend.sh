#!/bin/bash
# start_backend.sh
# This script starts the backend WebSocket server automatically.

# Start the WebSocket server in the background and log its output.
./backend/websocket_server > websocket.log 2>&1 &

# Optionally, start other backend processes here.
echo "WebSocket server started in the background."
