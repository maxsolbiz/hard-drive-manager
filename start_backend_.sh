#!/bin/bash
# start_backend.sh
# This script starts the backend WebSocket server and the FastAPI server automatically.

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

# Start the FastAPI server with uvicorn in the background and log its output.
echo "Starting FastAPI server..."
uvicorn api.app:app --reload --port 8000 > api.log 2>&1 &
API_PID=$!

echo "Both servers have been started."
echo "WebSocket PID: $WS_PID"
echo "FastAPI (uvicorn) PID: $API_PID"
echo "Press Ctrl+C to stop both servers."

# Wait indefinitely so the script doesn't exit.
wait
