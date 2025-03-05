import React, { useState, useEffect } from "react";
import Dashboard from "./Dashboard";         // Summary of detected drives (without health metrics)
import DetailedHealth from "./DetailedHealth";   // Component that fetches detailed health data from /detailed-health
import ToggleSwitch from "./ToggleSwitch";     // Your existing toggle switch component
import hardDriveIcon from "../assets/harddrive.png"; // Full path: ./frontend/src/assets/harddrive.png

const DashboardLayout = () => {
  const [darkMode, setDarkMode] = useState(() => localStorage.getItem("theme") === "dark");
  const [driveData, setDriveData] = useState([]);

  // Assume Dashboard fetches summary; here we use DetailedHealth API to get detailed info for each drive.
  // Alternatively, if your detailed health endpoint returns data for all drives,
  // you can fetch it once and then pass the data to each card.
  useEffect(() => {
    const fetchDetailedData = async () => {
      try {
        const res = await fetch("http://localhost:8001/detailed-health");
        const data = await res.json();
        // data.drives should be an array
        setDriveData(data.drives || []);
      } catch (error) {
        console.error("Error fetching detailed health data:", error);
      }
    };
    fetchDetailedData();
  }, []);

  const toggleDarkMode = () => {
    setDarkMode(!darkMode);
  };

  useEffect(() => {
    if (darkMode) {
      document.documentElement.classList.add("dark");
      localStorage.setItem("theme", "dark");
    } else {
      document.documentElement.classList.remove("dark");
      localStorage.setItem("theme", "light");
    }
  }, [darkMode]);

  return (
    <div className="min-h-screen bg-gray-50 dark:bg-gray-900 text-gray-900 dark:text-gray-100 flex flex-col">
      {/* Header */}
      <header className="flex justify-between items-center p-6 bg-white dark:bg-gray-800 shadow-md">
        <h1 className="text-3xl font-extrabold">Hard Drive Manager Dashboard</h1>
        <ToggleSwitch darkMode={darkMode} onToggle={toggleDarkMode} />
      </header>

      {/* Main Content */}
      <main className="flex-1 container mx-auto px-4 py-8">
        {/* Grid of drive cards; each card combines general and health details */}
        <div className="grid grid-cols-1 sm:grid-cols-2 lg:grid-cols-3 gap-6">
          {driveData.map((drive, index) => (
            <div
              key={index}
              className="bg-white dark:bg-gray-800 rounded-lg shadow-lg p-4 flex flex-col"
            >
              {/* Card Header */}
              <div className="flex items-center mb-4">
                <img
                  src={hardDriveIcon}
                  alt="Hard Drive Icon"
                  className="w-12 h-12 mr-4"
                />
                <h2 className="text-xl font-bold">
                {drive.drive_type?.trim()} 
                </h2>
              </div>
              {/* Card Body */}
              <div className="flex flex-col sm:flex-row gap-4">
                {/* Left Column: General Info (from drive detection summary) */}
                <div className="flex-1 border-r border-gray-200 dark:border-gray-700 pr-2">
                  <p><span className="font-medium">Device:</span> {drive.device}</p>
                 
                  <p><span className="font-medium">Model Family:</span> {drive.model_family}</p>
                  <p><span className="font-medium">Model:</span> {drive.device_model.trim()}</p>
                  <p><span className="font-medium">Serial:</span> {drive.serial_number?.trim()}</p>
                  <p><span className="font-medium">Size:</span> {drive.user_capacity}</p>
                </div>
                {/* Right Column: Health Details */}
                <div className="flex-1 pl-2">
                  <p><span className="font-medium">Health Status:</span> {drive.health_status?.trim()}</p>
                  <p><span className="font-medium">Temperature:</span> {drive.temperature}°C</p>
                  <p><span className="font-medium">Power On Hours:</span> {drive.power_on_hours}</p>
                  <p><span className="font-medium">Reallocated Sectors:</span> {drive.reallocated_sectors}</p>
                  <p><span className="font-medium">Form Factor:</span> {drive.form_factor?.trim()}</p>
                </div>
              </div>
            </div>
          ))}
          {/* If no drives or error, you can show a fallback card */}
          {driveData.length === 0 && (
            <div className="col-span-full text-center text-gray-500">
              No drive data available.
            </div>
          )}
        </div>
      </main>

      {/* Footer */}
      <footer className="bg-white dark:bg-gray-800 py-4 shadow-inner">
        <div className="container mx-auto text-center text-gray-600 dark:text-gray-400 text-sm">
          © 2025 <b>Rukn Al Amal TR.</b> All Rights Reserved.
        </div>
      </footer>
    </div>
  );
};

export default DashboardLayout;
