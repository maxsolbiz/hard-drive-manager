import React, { useState, useEffect } from "react";
import axios from "axios";

const Dashboard = () => {
  const [drives, setDrives] = useState([]);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState(null);

  useEffect(() => {
    axios
      .get(process.env.REACT_APP_BACKEND_URL + "/drives")
      .then((res) => {
        // Map to only summary fields:
        const summary = res.data.map((drive) => ({
          device: drive.device || drive.name,
          size: drive.size,
          model: drive.model || drive.device_model,
          vendor: drive.vendor,
          modelFamily: drive.modelFamily,
          serial: drive.serial || drive.serial_number,
        }));
        setDrives(summary);
        setLoading(false);
      })
      .catch((err) => {
        console.error("Error fetching drive summary:", err);
        setError(err);
        setLoading(false);
      });
  }, []);

  if (loading) return <p>Loading detected drives...</p>;
  if (error) return <p className="text-red-500">Error loading drives.</p>;

  return (
    <div className="space-y-4">
      <h2 className="text-xl font-bold mb-4">Drive Status</h2>
      {drives.map((drive, index) => (
        <div key={index} className="p-4 border border-gray-200 dark:border-gray-700 rounded">
          <h3 className="text-xl font-semibold">{drive.device}</h3>
          <p>
            <span className="font-medium">Size:</span> {drive.size}
          </p>
          <p>
            <span className="font-medium">Model:</span> {drive.model}
          </p>
          <p>
            <span className="font-medium">Vendor:</span> {drive.vendor}
          </p>
          <p>
            <span className="font-medium">Model Family:</span> {drive.modelFamily}
          </p>
          <p>
            <span className="font-medium">Serial:</span> {drive.serial}
          </p>
        </div>
      ))}
    </div>
  );
};

export default Dashboard;
