import React, { useEffect, useState } from 'react';
import { fetchDriveHealth } from '../api/apiClient';

const DriveCard = ({ drive }) => {
  const [health, setHealth] = useState(null);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState(null);

  // Function to fetch health data for the given drive.
  const getHealthData = async () => {
    try {
      const data = await fetchDriveHealth(drive.name);
      setHealth(data);
      setLoading(false);
    } catch (err) {
      setError(err);
      setLoading(false);
    }
  };

  useEffect(() => {
    getHealthData();
    // Poll for health updates every 10 seconds.
    const interval = setInterval(() => {
      getHealthData();
    }, 10000);
    return () => clearInterval(interval);
  }, [drive.name]);

  return (
    <div className="p-4 bg-white dark:bg-gray-800 rounded shadow flex flex-col md:flex-row">
      {/* Left Column: Drive Info */}
      <div className="flex-1 p-4 border-b md:border-b-0 md:border-r border-gray-300 dark:border-gray-600">
        <h3 className="font-bold text-lg mb-2">{drive.name}</h3>
        <p><span className="font-medium">Size:</span> {drive.size}</p>
        <p><span className="font-medium">Model:</span> {drive.model}</p>
        <p><span className="font-medium">Vendor:</span> {drive.vendor}</p>
        <p><span className="font-medium">Model Family:</span> {drive.modelFamily}</p>
        <p><span className="font-medium">Serial:</span> {drive.serial}</p>
      </div>
      {/* Right Column: Health Info */}
      <div className="flex-1 p-4">
        <h3 className="font-bold text-lg mb-2">Health Metrics</h3>
        {loading ? (
          <p>Loading health metrics...</p>
        ) : error ? (
          <p className="text-red-500">Error loading health data</p>
        ) : (
          <div>
            <p><span className="font-medium">Temperature:</span> {health.temperature}°C</p>
            <p><span className="font-medium">Read Errors:</span> {health.readErrorCount}</p>
            <p><span className="font-medium">Write Errors:</span> {health.writeErrorCount}</p>
            <p><span className="font-medium">Overall Health:</span> {health.overallHealth}</p>
            <p><span className="font-medium">SMART Status:</span> {health.smartStatus}</p>
          </div>
        )}
      </div>
    </div>
  );
};

export default DriveCard;
