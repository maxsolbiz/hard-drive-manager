import React from "react";
import { BrowserRouter, Routes, Route } from "react-router-dom";
import DriveCards from "./components/DriveCards";
import ErrorBoundary from "./ErrorBoundary";

function App() {
  return (
    <ErrorBoundary>
      <BrowserRouter>
        <Routes>
          <Route path="/" element={<DriveCards />} />
        </Routes>
      </BrowserRouter>
    </ErrorBoundary>
  );
}

export default App;
