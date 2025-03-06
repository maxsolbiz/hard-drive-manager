import React from "react";
import HDDIcon from "../assets/harddrive.png"; // Default HDD icon
import SSDIcon from "../assets/ssd.png";         // Icon for SSD
import NVMeIcon from "../assets/nvme.png";       // Icon for NVMe

const DashboardCard = ({ drive }) => {
  // Determine the icon based on drive health type if available.
  let driveIcon = HDDIcon;
  if (drive.health && drive.health.drive_type) {
    const type = drive.health.drive_type.toLowerCase().trim();
    if (type === "ssd") {
      driveIcon = SSDIcon;
    } else if (type === "nvme") {
      driveIcon = NVMeIcon;
    }
  }
  
  // Use form_factor from detection data; fallback to "3.5" if not provided.
  const formFactor =
    drive.form_factor && drive.form_factor.trim() !== "" ? drive.form_factor.trim() : "3.5";
  
  return (
    <div className="bg-white dark:bg-gray-800 rounded-lg shadow-lg p-6">
      {/* Header with icon and drive type and form factor */}
      <div className="flex items-center mb-4 border-b border-gray-200 dark:border-gray-700 pb-2">
        <img src={driveIcon} alt="Drive Icon" className="w-12 h-12 mr-4" />
        <h2 className="text-2xl font-bold">
          {drive.health ? drive.health.drive_type : "Unknown"} - {formFactor}"
        </h2>
      </div>
      
      {/* General Information Section */}
      <div className="mb-4">
        <h3 className="text-xl font-semibold mb-2 text-gray-700 dark:text-gray-200">General Info</h3>
        <p>
          <span className="font-medium">Device:</span> {drive.device || drive.name}
        </p>
        <p>
          <span className="font-medium">Size:</span> {drive.size}
        </p>
        <p>
          <span className="font-medium">Vendor:</span> {drive.vendor}
        </p>
        <p>
          <span className="font-medium">Model Family:</span> {drive.modelFamily}
        </p>
        <p>
          <span className="font-medium">Serial:</span> {drive.serial ? drive.serial.trim() : ""}
        </p>
      </div>
      
      {/* Health Details Section */}
      <div>
        <h3 className="text-xl font-semibold mb-2 text-gray-700 dark:text-gray-200">Health Details</h3>
        {drive.health ? (
          <>
            <p>
              <span className="font-medium">Health Status:</span>{" "}
              {drive.health.health_status?.trim()}
            </p>
            <p>
              <span className="font-medium">Temperature:</span>{" "}
              {drive.health.temperature ?? "N/A"}°C
            </p>
            <p>
              <span className="font-medium">Power On Hours:</span>{" "}
              {drive.health.power_on_hours ?? "N/A"}
            </p>
            <p>
              <span className="font-medium">Reallocated Sectors:</span>{" "}
              {drive.health.reallocated_sectors ?? "N/A"}
            </p>
            <p>
              <span className="font-medium">Firmware Version:</span>{" "}
              {drive.health.firmware_version ?? "N/A"}
            </p>
            <p>
              <span className="font-medium">Total LBAs Read:</span>{" "}
              {drive.health.total_lbas_read ?? "N/A"}
            </p>
            <p>
              <span className="font-medium">Total LBAs Written:</span>{" "}
              {drive.health.total_lbas_written ?? "N/A"}
            </p>
          </>
        ) : (
          <p className="text-red-500">Health data not available.</p>
        )}
      </div>
    </div>
  );
};

export default DashboardCard;
