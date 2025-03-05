import React, { useState, useEffect } from "react";
import axios from "axios";
import hardDriveIcon from "../assets/harddrive.png"; // Adjust path as needed

const DetailedHealth = () => {
  const [healthData, setHealthData] = useState(null);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState(null);

  useEffect(() => {
    // Fetch the detailed health data from the API endpoint.
    axios
      .get(process.env.REACT_APP_BACKEND_URL + "/detailed-health")
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

  return (
    <div className="p-4 bg-white dark:bg-gray-800 rounded shadow">
      <div className="flex items-center mb-4">
        <img src={hardDriveIcon} alt="Hard Drive" className="w-12 h-12 mr-4" />
        <h2 className="text-2xl font-bold">Health Details</h2>
      </div>
      {loading ? (
        <p>Loading health details...</p>
      ) : error ? (
        <p className="text-red-500">Error loading health details.</p>
      ) : healthData && healthData.drives ? (
        healthData.drives.map((drive, index) => (
          <div key={index} className="mb-4 border-b border-gray-300 pb-2">
            <h3 className="text-lg font-semibold">{drive.device}</h3>
            <p>
              <span className="font-medium">Temperature:</span> {drive.temperature}°C
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
            {/* Add more fields as desired */}
          </div>
        ))
      ) : (
        <p>No health details available.</p>
      )}
    </div>
  );
};

export default DetailedHealth;
