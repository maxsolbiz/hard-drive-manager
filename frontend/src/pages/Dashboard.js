import React from 'react';
import useWebSocket from '../hooks/useWebSocket';

const Dashboard = () => {
  const { socketData, status } = useWebSocket('ws://localhost:9002');

  return (
    <div className="p-6">
      <h1 className="text-3xl font-bold mb-4">Drive Monitor Dashboard</h1>
      <p className="mb-4">WebSocket Status: <span className="font-medium">{status}</span></p>
      {socketData ? (
        <pre className="bg-gray-100 p-4 rounded">{JSON.stringify(socketData, null, 2)}</pre>
      ) : (
        <p>No data received yet.</p>
      )}
    </div>
  );
};

export default Dashboard;
