import React, { useEffect, useState } from "react";

const Dashboard = () => {
  const [updates, setUpdates] = useState([]);

  useEffect(() => {
    const ws = new WebSocket("ws://localhost:8000/ws");
    ws.onopen = () => console.log("WebSocket connection established");
    ws.onmessage = (event) => {
      setUpdates(prev => [...prev, event.data]);
    };
    ws.onerror = (error) => console.error("WebSocket error:", error);
    return () => ws.close();
  }, []);

  return (
    <div className="container mx-auto p-4">
      <h1 className="text-2xl font-bold mb-4">Drive Monitoring Dashboard</h1>
      <div className="bg-gray-100 p-4 rounded shadow">
        <h2 className="text-xl mb-2">Live Updates:</h2>
        <ul className="list-disc ml-5">
          {updates.map((update, index) => (
            <li key={index} className="mb-1">{update}</li>
          ))}
        </ul>
      </div>
    </div>
  );
};

export default Dashboard;
