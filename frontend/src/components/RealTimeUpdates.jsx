// frontend/src/components/RealTimeUpdates.jsx
import React, { useEffect, useState } from 'react';
import websocketClient from '../api/websocketClient';

const RealTimeUpdates = () => {
  const [updates, setUpdates] = useState([]);

  useEffect(() => {
    const handleUpdate = (data) => {
      setUpdates((prev) => [data, ...prev]);
    };

    websocketClient.subscribe(handleUpdate);
    return () => {
      websocketClient.unsubscribe(handleUpdate);
    };
  }, []);

  return (
    <div>
      <h2 className="text-xl font-bold mb-4">Real-Time Updates</h2>
      <div className="overflow-auto h-64 bg-white dark:bg-gray-800 rounded shadow p-4">
        {updates.length === 0 && <p>No updates yet.</p>}
        {updates.map((update, index) => (
          <div key={index} className="border-b border-gray-300 dark:border-gray-600 py-2">
            <pre className="text-sm">{JSON.stringify(update, null, 2)}</pre>
          </div>
        ))}
      </div>
    </div>
  );
};

export default RealTimeUpdates;
