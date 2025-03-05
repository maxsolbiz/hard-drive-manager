import React, { useState, useEffect } from "react";
import axios from "axios";

const DetailedHealth = () => {
  const [healthData, setHealthData] = useState(null);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState(null);

  useEffect(() => {
    // Use the fixed URL for the detailed health API endpoint.
    axios
      .get("http://localhost:8001/detailed-health")
      .then((res) => {
        setHealthData(res.data);
        setLoading(false);
      })
      .catch((err) => {
        console.error("Error fetching detailed health data:", err);
        setError(err);
        setLoading(false);
      });
  }, []);

  if (loading) {
    return <p>Loading health details...</p>;
  }
  if (error) {
    return <p className="text-red-500">Error loading health details.</p>;
  }
  if (!healthData || !healthData.drives) {
    return <p>No detailed health data available.</p>;
  }

  return (
    <div className="space-y-4">
      {healthData.drives.map((drive, index) => (
        <div key={index} className="p-2 border border-gray-200 dark:border-gray-700 rounded">
          <h3 className="font-semibold">{drive.device}</h3>
          <p>
            <span className="font-medium">Temperature:</span> {drive.temperature}°C
          </p>
          <p>
            <span className="font-medium">Model Family</span> {drive.model_family}
          </p>
          <p>
            <span className="font-medium">Firmware Version</span> {drive.firmware_version}
          </p>
          <p>
            <span className="font-medium">Form Factor</span> {drive.form_factor}
          </p>
          <p>
            <span className="font-medium">Power On Hours:</span> {drive.power_on_hours}
          </p>
          <p>
            <span className="font-medium">Reallocated Sectors:</span> {drive.reallocated_sectors}
          </p>
          <p>
            <span className="font-medium">Health Status:</span> {drive.health_status}
          </p>
          {/* Add more fields if desired */}
        </div>
      ))}
    </div>
  );
};

export default DetailedHealth;
