import React from "react";
import SunIcon from "../assets/sun.png";
import MoonIcon from "../assets/moon.png";

const ToggleSwitch = ({ darkMode, onToggle }) => {
  return (
    <label className="relative inline-flex items-center cursor-pointer">
      <input
        type="checkbox"
        checked={darkMode}
        onChange={onToggle}
        className="sr-only peer"
      />
      <div className="w-11 h-6 bg-gray-300 rounded-full peer dark:bg-gray-600 peer-focus:outline-none peer-focus:ring-2 peer-focus:ring-blue-500 dark:peer-focus:ring-blue-300 transition-colors duration-300"></div>
      <div className="absolute inset-0 flex items-center justify-between px-1 pointer-events-none">
        <img src={SunIcon} alt="Light Mode" className="w-4 h-4" />
        <img src={MoonIcon} alt="Dark Mode" className="w-4 h-4" />
      </div>
      <div className="absolute top-[2px] left-[2px] w-5 h-5 bg-white rounded-full shadow-md transform transition-all duration-300 peer-checked:translate-x-[calc(100%-2px)]"></div>
    </label>
  );
};

export default ToggleSwitch;
