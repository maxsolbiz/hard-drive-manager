// src/components/HealthDetails.jsx
import React, { useState, useEffect } from "react";
import axios from "axios";

const HealthDetails = () => {
  const [data, setData] = useState(null);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState(null);

  useEffect(() => {
    axios
      .get(process.env.REACT_APP_BACKEND_URL + "/detailed-health")
      .then((res) => {
        setData(res.data);
        setLoading(false);
      })
      .catch((err) => {
        console.error("Error fetching detailed health data:", err);
        setError(err);
        setLoading(false);
      });
  }, []);

  return (
    <div className="p-4">
      {loading ? (
        <p>Loading health details...</p>
      ) : error ? (
        <p className="text-red-500">Error loading health details.</p>
      ) : data && data.drives ? (
        // Display drives in a simple list or any layout you prefer.
        data.drives.map((drive, index) => (
          <div key={index} className="mb-4 p-2 border border-gray-200 dark:border-gray-700 rounded">
            <h3 className="font-semibold">{drive.device}</h3>
            <p><span className="font-medium">Temperature:</span> {drive.temperature}°C</p>
            <p><span className="font-medium">Power On Hours:</span> {drive.power_on_hours}</p>
            <p><span className="font-medium">Reallocated Sectors:</span> {drive.reallocated_sectors}</p>
            <p><span className="font-medium">Health Status:</span> {drive.health_status}</p>
            {/* Add other attributes as needed */}
          </div>
        ))
      ) : (
        <p>No detailed health data available.</p>
      )}
    </div>
  );
};

export default HealthDetails;
