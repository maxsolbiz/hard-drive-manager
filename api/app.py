from fastapi import FastAPI, HTTPException
from pydantic import BaseModel
import json
import subprocess

app = FastAPI(title="Drive Health Recommendation API")

class HealthMetrics(BaseModel):
    driveName: str
    temperature: float
    readErrorCount: int
    writeErrorCount: int
    overallHealth: float
    smartStatus: str

def get_recommendation_from_model(health_json: str) -> str:
    # Call the ai_model.py script and pass the JSON file path.
    # For simplicity, we'll assume the script is in the same directory.
    try:
        result = subprocess.run(
            ["python3", "ai_model.py", health_json],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            check=True,
            text=True
        )
        return result.stdout.strip()
    except subprocess.CalledProcessError as e:
        raise HTTPException(status_code=500, detail=f"Model error: {e.stderr.strip()}")

@app.post("/recommendation")
def recommendation(metrics: HealthMetrics):
    # Convert the input to JSON string.
    health_json = metrics.json()
    recommendation_text = get_recommendation_from_model(health_json)
    return {"recommendation": recommendation_text}

@app.get("/")
def read_root():
    return {"message": "Welcome to the Drive Health Recommendation API"}

# Define your other endpoints, e.g., /recommendation
# ...
