# Use an official Ubuntu runtime as a parent image
FROM ubuntu:24.04

# Install required packages: build tools, cmake, git, Python3, pip, and SQLite3
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    python3 \
    python3-pip \
    sqlite3

# Install Uvicorn and FastAPI for the API server
RUN pip3 install uvicorn fastapi

# Set the working directory in the container
WORKDIR /app

# Copy the project files into the container
COPY . .

# Build the project
RUN mkdir build && cd build && cmake .. && make

# Expose port 8000 for the API server (if needed)
EXPOSE 8000

# Command to run the API server
CMD ["uvicorn", "api/app:app", "--host", "0.0.0.0", "--port", "8000"]
