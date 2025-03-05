#!/bin/bash
# start_backend.sh
# This script starts the backend WebSocket server, the original FastAPI API, 
# and the new Detailed Health API in separate processes.

# Ensure the script runs from the project root
cd "$(dirname "$0")"

# Activate the virtual environment
if [ -f "ai-env/bin/activate" ]; then
    echo "Activating virtual environment..."
    source ai-env/bin/activate
else
    echo "Virtual environment not found. Please ensure ai-env exists."
    exit 1
fi

# Start the WebSocket server in the background and log its output.
echo "Starting WebSocket server..."
./backend/websocket_server > websocket.log 2>&1 &
WS_PID=$!

# Start the original FastAPI API (api/app:app) on port 8000 in the background.
echo "Starting original FastAPI server on port 8000..."
uvicorn api.app:app --reload --port 8000 > api.log 2>&1 &
API_OLD_PID=$!

# Start the new Detailed Health API (api/detailed_health_api:app) on port 8001 in the background.
echo "Starting Detailed Health API on port 8001..."
uvicorn api.detailed_health_api:app --reload --port 8001 > detailed_api.log 2>&1 &
API_NEW_PID=$!

echo "All servers have been started."
echo "WebSocket PID: $WS_PID"
echo "Original FastAPI (port 8000) PID: $API_OLD_PID"
echo "Detailed Health API (port 8001) PID: $API_NEW_PID"
echo "Press Ctrl+C to stop all servers."

# Wait indefinitely so the script doesn't exit.
wait
