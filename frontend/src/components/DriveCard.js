import React from 'react';

const DriveCard = ({ drive }) => {
  return (
    <div className="border rounded-lg shadow p-4 bg-white dark:bg-gray-800">
      <h2 className="text-xl font-semibold mb-2">Drive: {drive.serialNumber}</h2>
      <p>Health: {drive.healthStatus}</p>
      <p>Temperature: {drive.temperature}°C</p>
      {/* Add additional metrics here */}
    </div>
  );
};

export default DriveCard;
