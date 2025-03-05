from fastapi import FastAPI, HTTPException
from fastapi.middleware.cors import CORSMiddleware
import subprocess
import os
import json



app = FastAPI(title="Detailed Health API")

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

@app.get("/detailed-health")
def get_detailed_health():
    try:
        BASE_DIR = os.path.dirname(os.path.abspath(__file__))
        # Adjust the path to where your executable is built.
        exec_path = os.path.join(BASE_DIR, "..", "backend", "build", "health_monitor_full")
        cwd_path = os.path.join(BASE_DIR, "..", "backend", "build")
        
        print("Using executable path:", exec_path)
        print("Using working directory:", cwd_path)
        
        result = subprocess.run([exec_path], capture_output=True, text=True, cwd=cwd_path)
        stdout = result.stdout.strip()
        stderr = result.stderr.strip()
        
        print("Return Code:", result.returncode)
        print("STDOUT:", stdout)
        print("STDERR:", stderr)
        
        if result.returncode != 0:
            raise Exception(f"Executable returned non-zero exit code {result.returncode}. STDERR: {stderr}")
        
        if not stdout:
            raise Exception("No output received from executable. STDERR: " + stderr)
        
        # Find the beginning of the JSON output (first '{')
        json_start = stdout.find('{')
        if json_start == -1:
            raise Exception("JSON start not found in output. Full output: " + stdout)
        json_str = stdout[json_start:]
        
        # Parse the JSON
        data = json.loads(json_str)
        return data
    except Exception as e:
        raise HTTPException(status_code=500, detail="Error fetching detailed health data: " + str(e))


if __name__ == "__main__":
    import uvicorn
    uvicorn.run(app, host="127.0.0.1", port=8001)
