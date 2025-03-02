import React from "react";
import { BrowserRouter, Routes, Route } from "react-router-dom";
import Dashboard from "./components/Dashboard";
import RecommendationPage from "./components/RecommendationPage";

function App() {
  return (
    <BrowserRouter>
      <Routes>
        <Route path="/" element={<Dashboard />} />
        <Route path="/recommendation" element={<RecommendationPage />} />
      </Routes>
    </BrowserRouter>
  );
}

export default App;
