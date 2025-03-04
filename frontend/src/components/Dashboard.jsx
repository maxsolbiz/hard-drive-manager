// frontend/src/components/Dashboard.jsx
import React, { useEffect, useState } from 'react';
import { fetchDrives } from '../api/apiClient';
import DriveCard from './DriveCard.jsx';


const Dashboard = () => {
  const [drives, setDrives] = useState([]);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState(null);

  useEffect(() => {
    fetchDrives()
      .then((data) => {
        console.log("Fetched drives:", data);
        setDrives(data);
        setLoading(false);
      })
      .catch((err) => {
        console.error("Error fetching drives:", err);
        setError(err);
        setLoading(false);
      });
  }, []);

  if (loading) return <div>Loading drive information...</div>;
  if (error)
    return (
      <div className="text-red-500">
        Error loading data: {error.message || JSON.stringify(error)}
      </div>
    );

  return (
    <div>
      <h2 className="text-xl font-bold mb-4">Drive Status</h2>
      <div className="grid grid-cols-1 gap-4">
        {drives.map((drive, index) => (
          <DriveCard key={index} drive={drive} />
        ))}
      </div>
    </div>
  );
};

export default Dashboard;
