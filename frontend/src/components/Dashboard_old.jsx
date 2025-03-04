import React, { useEffect, useState } from "react";

const Dashboard = () => {
  const [updates, setUpdates] = useState([]);
  const [connectionStatus, setConnectionStatus] = useState("Disconnected");

  useEffect(() => {
    const socket = new WebSocket("ws://127.0.0.1:9002/ws");
    
    socket.onopen = () => {
      console.log("WebSocket connection established");
      setConnectionStatus("Connected");
    };

    socket.onmessage = (event) => {
      console.log("Received message:", event.data);
      setUpdates(prev => [...prev, event.data]);
    };

    socket.onerror = (error) => {
      console.error("WebSocket error:", error);
      setConnectionStatus("Error");
    };

    socket.onclose = () => {
      console.log("WebSocket connection closed");
      setConnectionStatus("Disconnected");
    };

    return () => {
      socket.close();
    };
  }, []);

  return (
    <div className="container mx-auto p-4">
      <h1 className="text-2xl font-bold mb-4">Drive Monitoring Dashboard</h1>
      <p className="mb-4">
        WebSocket Status: <span className="font-semibold">{connectionStatus}</span>
      </p>
      <div className="bg-gray-100 p-4 rounded shadow">
        <h2 className="text-xl mb-2">Live Updates:</h2>
        {updates.length === 0 ? (
          <p>No updates yet.</p>
        ) : (
          <ul className="list-disc ml-5">
            {updates.map((update, index) => (
              <li key={index} className="mb-1">{update}</li>
            ))}
          </ul>
        )}
      </div>
    </div>
  );
};

export default Dashboard;
