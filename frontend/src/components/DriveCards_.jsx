import React, { useEffect, useState } from "react";
import ToggleSwitch from "./ToggleSwitch";
import DriveCard from "./DriveCard";

const DriveCards = () => {
  const [drives, setDrives] = useState([]);
  const [loading, setLoading] = useState(true);
  const [darkMode, setDarkMode] = useState(() => {
    const savedTheme = localStorage.getItem("darkMode");
    return savedTheme ? JSON.parse(savedTheme) : false;
  });
  const [searchTerm, setSearchTerm] = useState("");
  const [selectedVendor, setSelectedVendor] = useState("All");
  const [vendors, setVendors] = useState([]);

  useEffect(() => {
    fetch("http://127.0.0.1:8000/drives")
      .then((response) => response.json())
      .then((data) => {
        console.log("Fetched drives:", data);
        if (Array.isArray(data)) {
          setDrives(data);
          const uniqueVendors = Array.from(new Set(data.map((drive) => drive.vendor)));
          setVendors(uniqueVendors);
        } else {
          console.error("Expected an array of drives, but got:", data);
        }
        setLoading(false);
      })
      .catch((error) => {
        console.error("Error fetching drives:", error);
        setLoading(false);
      });
  }, []);

  useEffect(() => {
    localStorage.setItem("darkMode", darkMode);
  }, [darkMode]);

  const toggleTheme = () => {
    setDarkMode((prevMode) => !prevMode);
  };

  const filteredDrives = drives.filter((drive) => {
    const matchesName = drive.name.toLowerCase().includes(searchTerm.toLowerCase());
    const matchesVendor = selectedVendor === "All" || drive.vendor === selectedVendor;
    return matchesName && matchesVendor;
  });

  return (
    <div
      className={
        darkMode
          ? "bg-gray-900 text-white min-h-screen transition-colors duration-300"
          : "bg-gray-100 text-gray-900 min-h-screen transition-colors duration-300"
      }
    >
      <div className="container mx-auto py-10 px-4">
        <header className="flex flex-col sm:flex-row justify-between items-center mb-8">
          <h1 className="text-4xl font-extrabold mb-4 sm:mb-0">
            Attached Hard Drives
          </h1>
          <div className="flex flex-col sm:flex-row items-center space-y-4 sm:space-y-0 sm:space-x-4">
            <input
              type="text"
              placeholder="Search drives..."
              className="px-4 py-2 border rounded focus:outline-none"
              value={searchTerm}
              onChange={(e) => setSearchTerm(e.target.value)}
            />
            <select
              className={`px-4 py-2 rounded focus:outline-none ${darkMode ? "bg-gray-800 text-white border-gray-600" : "bg-white text-gray-900 border-gray-300"}`}
              value={selectedVendor}
              onChange={(e) => setSelectedVendor(e.target.value)}
            >
              <option value="All">All Vendors</option>
              {vendors.map((vendor) => (
                <option key={vendor} value={vendor}>
                  {vendor}
                </option>
              ))}
            </select>

            <ToggleSwitch darkMode={darkMode} onToggle={toggleTheme} />
          </div>
        </header>
        {loading ? (
          <p className="text-xl">Loading drives...</p>
        ) : filteredDrives.length === 0 ? (
          <p className="text-xl">No drives detected.</p>
        ) : (
          <div className="grid grid-cols-1 sm:grid-cols-2 md:grid-cols-3 gap-8">
            {filteredDrives.map((drive) => (
              <DriveCard key={drive.name} drive={drive} darkMode={darkMode} />
            ))}
          </div>
        )}
      </div>
    </div>
  );
};

export default DriveCards;
