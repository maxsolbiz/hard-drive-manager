import React, { useState, useEffect } from 'react';

function App() {
  // State to toggle dark mode
  const [darkMode, setDarkMode] = useState(false);

  // When darkMode changes, add or remove the 'dark' class on the document element
  useEffect(() => {
    if (darkMode) {
      document.documentElement.classList.add('dark');
    } else {
      document.documentElement.classList.remove('dark');
    }
  }, [darkMode]);

  return (
    <div className="min-h-screen bg-gray-100 dark:bg-gray-900 transition-colors duration-300">
      {/* Header */}
      <header className="bg-white dark:bg-gray-800 shadow">
        <div className="max-w-7xl mx-auto px-4 sm:px-6 lg:px-8 flex justify-between items-center py-4">
          <h1 className="text-2xl font-bold text-gray-900 dark:text-white">
            Drive Monitor Dashboard
          </h1>
          <button
            onClick={() => setDarkMode(!darkMode)}
            className="bg-blue-500 text-white px-4 py-2 rounded hover:bg-blue-600 transition"
          >
            {darkMode ? 'Light Mode' : 'Dark Mode'}
          </button>
        </div>
      </header>

      {/* Main Content */}
      <main className="max-w-7xl mx-auto p-6">
        {/* Section: Real-Time Drive Data */}
        <section className="mb-8">
          <h2 className="text-xl font-semibold text-gray-800 dark:text-gray-200">
            Real-Time Drive Data
          </h2>
          <div className="mt-4 grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-6">
            {/* Drive Card 1 */}
            <div className="bg-white dark:bg-gray-800 shadow rounded-lg p-6">
              <h3 className="text-lg font-bold text-gray-900 dark:text-white">
                Drive A
              </h3>
              <p className="mt-2 text-gray-700 dark:text-gray-300">
                Health: <span className="font-medium">Good</span>
              </p>
              <p className="mt-1 text-gray-700 dark:text-gray-300">
                Temperature: 35°C
              </p>
            </div>
            {/* Drive Card 2 */}
            <div className="bg-white dark:bg-gray-800 shadow rounded-lg p-6">
              <h3 className="text-lg font-bold text-gray-900 dark:text-white">
                Drive B
              </h3>
              <p className="mt-2 text-gray-700 dark:text-gray-300">
                Health: <span className="font-medium">Moderate</span>
              </p>
              <p className="mt-1 text-gray-700 dark:text-gray-300">
                Temperature: 40°C
              </p>
            </div>
            {/* Drive Card 3 */}
            <div className="bg-white dark:bg-gray-800 shadow rounded-lg p-6">
              <h3 className="text-lg font-bold text-gray-900 dark:text-white">
                Drive C
              </h3>
              <p className="mt-2 text-gray-700 dark:text-gray-300">
                Health: <span className="font-medium">Critical</span>
              </p>
              <p className="mt-1 text-gray-700 dark:text-gray-300">
                Temperature: 50°C
              </p>
            </div>
          </div>
        </section>

        {/* Section: AI Recommendations */}
        <section className="mb-8">
          <h2 className="text-xl font-semibold text-gray-800 dark:text-gray-200">
            AI Recommendations
          </h2>
          <div className="mt-4 bg-white dark:bg-gray-800 shadow rounded-lg p-6">
            <p className="text-gray-700 dark:text-gray-300">
              Replace Drive C soon. Consider backing up data immediately and running a detailed scan.
            </p>
          </div>
        </section>
      </main>

      {/* Footer */}
      <footer className="bg-white dark:bg-gray-800 shadow">
        <div className="max-w-7xl mx-auto px-4 py-4 text-center text-gray-600 dark:text-gray-400">
          © {new Date().getFullYear()} Drive Manager. All rights reserved.
        </div>
      </footer>
    </div>
  );
}

export default App;
