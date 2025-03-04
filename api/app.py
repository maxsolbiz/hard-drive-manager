import os
import subprocess
import json
import asyncio
from fastapi import FastAPI, WebSocket, HTTPException, Path
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel

app = FastAPI(title="Drive Health Recommendation API")

# Enable CORS to allow cross-origin requests (adjust origins as needed)
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

def extract_json_drive_data(output: str):
    """
    Extracts JSON objects from the output that are printed after the "Detected Drives:" header.
    This is used for the drive detection module when extra log lines exist.
    """
    lines = output.splitlines()
    drives = []
    start = False
    for line in lines:
        stripped = line.strip()
        if stripped == "Detected Drives:":
            start = True
            continue
        # Stop if we reach another header or log marker.
        if start and stripped and (stripped.endswith(":") or stripped.startswith("[INFO]")):
            break
        if start and stripped:
            try:
                drive = json.loads(stripped)
                drives.append(drive)
            except json.JSONDecodeError:
                pass
    return drives

def run_module(module: str, extra_args: list = []):
    try:
        BASE_DIR = os.path.dirname(os.path.abspath(__file__))
        # Path to the executable in backend/build
        exec_path = os.path.join(BASE_DIR, "..", "backend", "build", "hard_drive_manager")
        # Set working directory to the project root (one level above the api folder)
        cwd_path = os.path.join(BASE_DIR, "..")
        cmd = [exec_path, "--json", f"--module={module}"] + extra_args
        
        print(f"Running command: {' '.join(cmd)} with cwd: {cwd_path}")  # Debug log
        
        result = subprocess.run(cmd, capture_output=True, text=True, check=True, cwd=cwd_path)
        output = result.stdout.strip()
        if not output:
            error_detail = f"No output from executable. Stderr: {result.stderr}"
            print(error_detail)
            raise Exception(error_detail)
        
        # Try to parse the entire output as JSON.
        try:
            data = json.loads(output)
            return data
        except json.JSONDecodeError:
            # If the whole output isn't valid JSON, attempt to extract JSON objects line by line.
            data = extract_json_drive_data(output)
            return data
    except Exception as e:
        error_msg = f"Error running module '{module}': {e}"
        print(error_msg)
        raise HTTPException(status_code=500, detail=error_msg)

# --------------------------------------
# Endpoints for Each Module (Standalone)
# --------------------------------------

# 1. Drive Detection
@app.get("/drives")
def get_drives():
    return run_module("detect")

# 2. Scanning
@app.get("/scan/{drive_name}")
def scan_drive(drive_name: str = Path(..., description="Name of the drive to scan")):
    # Pass the drive name as an extra argument.
    return run_module("scan", [drive_name])

# 3. Drive Repair
@app.get("/repair/{drive_name}")
def repair_drive(
    drive_name: str = Path(..., description="Name of the drive to repair"),
    dry_run: bool = True
):
    # Pass drive name and mode flag.
    return run_module("repair", [drive_name, "dry_run" if dry_run else "live"])

# 4. Drive Cloning
class CloneRequest(BaseModel):
    source: str
    destination: str
    dry_run: bool = True

@app.post("/clone")
def clone_drive(req: CloneRequest):
    return run_module("clone", [req.source, req.destination, "dry_run" if req.dry_run else "live"])

# 5. Partitioning & Formatting
class PartitionRequest(BaseModel):
    drive_name: str
    size: float         # Partition size in GB
    file_system: str    # File system type, e.g. "EXT4"

@app.post("/partition")
def partition_drive(req: PartitionRequest, dry_run: bool = True):
    return run_module("partition", [req.drive_name, str(req.size), req.file_system, "dry_run" if dry_run else "live"])

# 6. AI Recommendation
class HealthMetrics(BaseModel):
    driveName: str
    temperature: float
    readErrorCount: int
    writeErrorCount: int
    overallHealth: float
    smartStatus: str

@app.post("/recommendation")
def get_recommendation(metrics: HealthMetrics):
    health_json = metrics.json()
    return run_module("ai", [health_json])

# 7. Logging/Reporting
@app.get("/logs")
def get_logs():
    return run_module("logs")

# 8. Health Monitoring
@app.get("/health/{drive_name}")
def health_drive(drive_name: str = Path(..., description="Name of the drive to monitor")):
    try:
        BASE_DIR = os.path.dirname(os.path.abspath(__file__))
        # Path to your built executable
        exec_path = os.path.join(BASE_DIR, "..", "backend", "build", "hard_drive_manager")
        cwd_path = os.path.join(BASE_DIR, "..")
        # Call the executable with the 'health' module and pass the drive name as argument.
        result = subprocess.run(
            [exec_path, "--json", "--module=health", drive_name],
            capture_output=True,
            text=True,
            check=True,
            cwd=cwd_path
        )
        output = result.stdout.strip()
        if not output:
            raise Exception("No output. Stderr: " + result.stderr)
        # Assume the output is valid JSON.
        health_data = json.loads(output)
        return health_data
    except Exception as e:
        raise HTTPException(status_code=500, detail="Error fetching health metrics: " + str(e))

# --------------------------------------
# WebSocket for Real-Time Drive Updates
# --------------------------------------
@app.websocket("/ws/drives")
async def websocket_drives(websocket: WebSocket):
    await websocket.accept()
    try:
        while True:
            # Get real-time detection data.
            data = run_module("detect")
            await websocket.send_text(json.dumps(data))
            await asyncio.sleep(10)
    except Exception as e:
        await websocket.close()
        print("WebSocket error:", e)

# --------------------------------------
# WebSocket for Real-Time Health Updates
# --------------------------------------        
@app.websocket("/ws/health/{drive_name}")
async def websocket_health(websocket: WebSocket, drive_name: str):
    await websocket.accept()
    try:
        while True:
            # Call the health endpoint logic
            try:
                BASE_DIR = os.path.dirname(os.path.abspath(__file__))
                exec_path = os.path.join(BASE_DIR, "..", "backend", "build", "hard_drive_manager")
                cwd_path = os.path.join(BASE_DIR, "..")
                result = subprocess.run(
                    [exec_path, "--json", "--module=health", drive_name],
                    capture_output=True,
                    text=True,
                    check=True,
                    cwd=cwd_path
                )
                output = result.stdout.strip()
                health_data = json.loads(output)
            except Exception as e:
                health_data = {"error": str(e)}
            await websocket.send_text(json.dumps(health_data))
            await asyncio.sleep(10)
    except Exception as e:
        await websocket.close()

