#!/usr/bin/env python3
import sys
import json

def main():
    if len(sys.argv) < 2:
        print("No file provided")
        return

    file_path = sys.argv[1]
    try:
        with open(file_path, 'r') as f:
            data = f.read()
        health_data = json.loads(data)
    except Exception as e:
        print("Error parsing input JSON")
        return

    overall_health = health_data.get("overallHealth", 100)
    temperature = health_data.get("temperature", 25)

    if overall_health < 80:
        recommendation = "Replace Drive Soon"
    elif temperature > 50:
        recommendation = "Backup Data Immediately"
    else:
        recommendation = "Drive is healthy"
    
    print(recommendation)

if __name__ == "__main__":
    main()
