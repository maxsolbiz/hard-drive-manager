// frontend/src/components/DriveHealth.jsx
import React, { useEffect, useState } from 'react';
import { fetchDriveHealth } from '../api/apiClient';

const DriveHealth = ({ driveName }) => {
  const [health, setHealth] = useState(null);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState(null);

  useEffect(() => {
    fetchDriveHealth(driveName)
      .then((data) => {
        setHealth(data);
        setLoading(false);
      })
      .catch((err) => {
        console.error("Error fetching drive health:", err);
        setError(err);
        setLoading(false);
      });
  }, [driveName]);

  if (loading) return <div>Loading health metrics...</div>;
  if (error)
    return (
      <div className="text-red-500">
        Error loading health data: {error.message || JSON.stringify(error)}
      </div>
    );

  return (
    <div className="p-4 bg-white dark:bg-gray-800 rounded shadow">
      <h3 className="font-bold text-lg mb-2">Health Metrics for {driveName}</h3>
      <p>
        <span className="font-medium">Temperature:</span> {health.temperature}°C
      </p>
      <p>
        <span className="font-medium">Read Errors:</span> {health.readErrorCount}
      </p>
      <p>
        <span className="font-medium">Write Errors:</span> {health.writeErrorCount}
      </p>
      <p>
        <span className="font-medium">Overall Health:</span> {health.overallHealth}
      </p>
      <p>
        <span className="font-medium">SMART Status:</span> {health.smartStatus}
      </p>
    </div>
  );
};

export default DriveHealth;
