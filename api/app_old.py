import os
import subprocess
import json
import asyncio
from fastapi import FastAPI, WebSocket, HTTPException
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel

app = FastAPI(title="Drive Health Recommendation API")

# Enable CORS to allow cross-origin requests from your frontend.
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],  # For production, restrict this to your frontend URL.
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

def extract_json_drive_data(output: str):
    """
    Extracts JSON objects from the output that appear between
    the "Detected Drives:" and the next header (e.g. "Scanning Drives:").
    """
    lines = output.splitlines()
    drives = []
    start = False
    for line in lines:
        stripped = line.strip()
        # Check for the start marker.
        if stripped == "Detected Drives:":
            start = True
            continue
        # End when we reach a header that isn't JSON (e.g., "Scanning Drives:")
        if start and stripped and (stripped.endswith(":") or stripped.startswith("[INFO]")):
            break
        # If we're in the JSON section, try to parse the line.
        if start and stripped:
            try:
                drive = json.loads(stripped)
                drives.append(drive)
            except json.JSONDecodeError:
                # Skip lines that are not valid JSON.
                pass
    return drives

@app.get("/drives")
def get_drives():
    try:
        # Compute an absolute path to the backend executable located in the build directory.
        BASE_DIR = os.path.dirname(os.path.abspath(__file__))
        exec_path = os.path.join(BASE_DIR, "..", "backend", "build", "hard_drive_manager")
        
        # Set the working directory to the project root (one level above the "api" folder).
        cwd_path = os.path.join(BASE_DIR, "..")
        
        # Call the backend executable with the --json flag for real drive data.
        result = subprocess.run(
            [exec_path, "--json"],
            capture_output=True,
            text=True,
            check=True,
            cwd=cwd_path  # set the working directory
        )
        output = result.stdout.strip()
        if not output:
            raise HTTPException(
                status_code=500,
                detail="Executable did not produce output. Stderr: " + result.stderr
            )
        
        # Extract JSON drive data from the output.
        drives_data = extract_json_drive_data(output)
        return drives_data
    except Exception as e:
        raise HTTPException(status_code=500, detail="Error fetching drive data: " + str(e))

# -------------------------
# AI Recommendation Endpoint
# -------------------------
class HealthMetrics(BaseModel):
    driveName: str
    temperature: float
    readErrorCount: int
    writeErrorCount: int
    overallHealth: float
    smartStatus: str

def get_recommendation_from_model(health_json: str) -> str:
    try:
        result = subprocess.run(
            ["python3", "ai_model.py", health_json],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            check=True,
            text=True,
        )
        return result.stdout.strip()
    except subprocess.CalledProcessError as e:
        raise HTTPException(status_code=500, detail=f"Model error: {e.stderr.strip()}")

@app.post("/recommendation")
def recommendation(metrics: HealthMetrics):
    health_json = metrics.json()
    recommendation_text = get_recommendation_from_model(health_json)
    return {"recommendation": recommendation_text}

# -------------------------
# WebSocket Endpoint for Drive Updates
# -------------------------
@app.websocket("/ws/drives")
async def websocket_drives(websocket: WebSocket):
    await websocket.accept()
    try:
        while True:
            # Fetch real drive data from the executable.
            try:
                drive_data = get_drives()  # This will call the /drives endpoint function.
            except HTTPException as e:
                drive_data = {"error": e.detail}
            await websocket.send_text(json.dumps(drive_data))
            await asyncio.sleep(10)
    except Exception as e:
        print("WebSocket error:", e)
