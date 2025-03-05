import React from 'react';

const DriveCard = ({ drive, icon }) => {
  return (
    <div className="bg-white rounded-lg shadow-md p-6 flex">
      <img src={icon} alt="Hard Drive Icon" className="w-16 h-16 mr-4" />
      <div className="flex-1 grid grid-cols-2 gap-4">
        {/* General Information */}
        <div>
          <h2 className="text-xl font-semibold mb-2">{drive.drive_type}</h2>
          <p><span className="font-medium">Device:</span> {drive.device}</p>
          <p><span className="font-medium">Type:</span> {drive.drive_type}</p>
          <p><span className="font-medium">Capacity:</span> {drive.user_capacity}</p>
          <p><span className="font-medium">Serial Number:</span> {drive.serial_number.trim()}</p>
        </div>
        {/* Health Details */}
        <div>
          <h3 className="text-lg font-semibold mb-2">Health Details</h3>
          <p><span className="font-medium">Health Status:</span> {drive.health_status.trim()}</p>
          <p><span className="font-medium">Temperature:</span> {drive.temperature}°C</p>
          <p><span className="font-medium">Power-On Hours:</span> {drive.power_on_hours}</p>
          <p><span className="font-medium">Reallocated Sectors:</span> {drive.reallocated_sectors}</p>
        </div>
      </div>
    </div>
  );
};

export default DriveCard;
