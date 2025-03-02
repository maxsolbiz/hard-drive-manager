from fastapi import FastAPI, WebSocket, HTTPException
from pydantic import BaseModel
import asyncio
import json
import subprocess

app = FastAPI(title="Drive Health Recommendation API")

# AI Recommendation Model Endpoint

class HealthMetrics(BaseModel):
    driveName: str
    temperature: float
    readErrorCount: int
    writeErrorCount: int
    overallHealth: float
    smartStatus: str

def get_recommendation_from_model(health_json: str) -> str:
    try:
        # Call the ai_model.py script with the JSON data passed as an argument.
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
    health_json = metrics.json()
    recommendation_text = get_recommendation_from_model(health_json)
    return {"recommendation": recommendation_text}

# WebSocket Endpoint for real-time updates

@app.websocket("/ws")
async def websocket_endpoint(websocket: WebSocket):
    await websocket.accept()
    try:
        while True:
            # For demonstration, send a heartbeat message every 5 seconds.
            await websocket.send_text("Heartbeat: update from API")
            await asyncio.sleep(5)
    except Exception as e:
        print("WebSocket error:", e)
