// frontend/src/components/DashboardLayout.jsx
import React, { useState, useEffect } from 'react';
import Dashboard from './Dashboard';
import ToggleSwitch from './ToggleSwitch';

const DashboardLayout = () => {
  const [darkMode, setDarkMode] = useState(() => localStorage.getItem('theme') === 'dark');

  useEffect(() => {
    if (darkMode) {
      document.documentElement.classList.add('dark');
      localStorage.setItem('theme', 'dark');
    } else {
      document.documentElement.classList.remove('dark');
      localStorage.setItem('theme', 'light');
    }
  }, [darkMode]);

  return (
    <div className="min-h-screen flex flex-col bg-gray-100 dark:bg-gray-900 text-gray-900 dark:text-gray-100">
      {/* Header */}
      <header className="relative flex justify-between items-center p-4 shadow bg-white dark:bg-gray-800">
        <h1 className="text-2xl font-bold">Hard Drive Manager Dashboard</h1>
        <div className="absolute right-4 top-1/2 transform -translate-y-1/2">
          <ToggleSwitch darkMode={darkMode} onToggle={() => setDarkMode(!darkMode)} />
        </div>
      </header>

      {/* Main Content */}
      <main className="flex-grow p-4">
        <Dashboard />
      </main>

      {/* Footer */}
      <footer className="p-4 bg-white dark:bg-gray-800 text-center text-sm">
        <p>© 2025 Your Company Name. All Rights Reserved.</p>
      </footer>
    </div>
  );
};

export default DashboardLayout;
