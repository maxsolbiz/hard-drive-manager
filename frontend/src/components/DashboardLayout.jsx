import React, { useState, useEffect } from "react";
import ToggleSwitch from "./ToggleSwitch";
import DashboardCard from "./DashboardCard";

const DashboardLayout = () => {
  const [darkMode, setDarkMode] = useState(() => localStorage.getItem("theme") === "dark");
  const [detectionData, setDetectionData] = useState([]);
  const [healthData, setHealthData] = useState([]);

  // Fetch drive detection data
  useEffect(() => {
    const fetchDetectionData = async () => {
      try {
        const res = await fetch("http://localhost:8000/drives");
        const data = await res.json();
        console.log("Detection Data:", data);
        setDetectionData(data.drives || data);
      } catch (error) {
        console.error("Error fetching drive detection data:", error);
      }
    };
    fetchDetectionData();
  }, []);

  // Fetch detailed health data
  useEffect(() => {
    const fetchHealthData = async () => {
      try {
        const res = await fetch("http://localhost:8001/detailed-health");
        const data = await res.json();
        console.log("Health Data:", data);
        setHealthData(data.drives || data);
      } catch (error) {
        console.error("Error fetching drive health data:", error);
      }
    };
    fetchHealthData();
  }, []);

  // Normalize device names to merge detection and health data
  const normalizeDevice = (device) =>
    (device || "").toLowerCase().replace(/^\/dev\//, "").trim();

  const combinedDriveData = detectionData.map((detection) => {
    const detectionDevice = normalizeDevice(detection.device || detection.name);
    const health = healthData.find((h) => normalizeDevice(h.device) === detectionDevice);
    if (!health) {
      console.warn(`No health data found for device: "${detectionDevice}"`);
    }
    return { ...detection, health };
  });

  const toggleDarkMode = () => setDarkMode(!darkMode);

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
      {/* Sticky Header */}
      <header className="sticky top-0 z-50 flex justify-between items-center p-6 bg-white dark:bg-gray-800 shadow-md">
        <h1 className="text-3xl font-extrabold">Hard Drive Manager Dashboard</h1>
        <ToggleSwitch darkMode={darkMode} onToggle={toggleDarkMode} />
      </header>

      {/* Main Content: Grid of drive cards */}
      <main className="container mx-auto px-4 py-8">
        <div className="grid grid-cols-1 sm:grid-cols-2 lg:grid-cols-3 gap-6">
          {combinedDriveData.map((drive, index) => (
            <DashboardCard key={index} drive={drive} />
          ))}
          {combinedDriveData.length === 0 && (
            <p className="col-span-full text-center text-gray-500">No drive data available.</p>
          )}
        </div>
      </main>

      {/* Footer */}
      <footer className="bg-white dark:bg-gray-800 py-4 shadow-inner">
        <div className="container mx-auto text-center text-gray-600 dark:text-gray-400 text-sm">
          © 2025 <b>Rukn AlAmal TR.</b> All Rights Reserved.
        </div>
      </footer>
    </div>
  );
};

export default DashboardLayout;
