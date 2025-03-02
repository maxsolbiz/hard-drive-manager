import React, { useState } from "react";

const RecommendationPage = () => {
  const [recommendation, setRecommendation] = useState("");
  const [loading, setLoading] = useState(false);

  const fetchRecommendation = async () => {
    setLoading(true);
    try {
      const response = await fetch("http://127.0.0.1:8000/recommendation", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({
          driveName: "sda",
          temperature: 35.5,
          readErrorCount: 0,
          writeErrorCount: 0,
          overallHealth: 98.0,
          smartStatus: "PASSED"
        }),
      });
      const data = await response.json();
      setRecommendation(data.recommendation);
    } catch (error) {
      setRecommendation("Error fetching recommendation");
    }
    setLoading(false);
  };

  return (
    <div className="container mx-auto p-4">
      <h1 className="text-2xl font-bold mb-4">AI Recommendation</h1>
      <button
        onClick={fetchRecommendation}
        className="bg-blue-500 text-white px-4 py-2 rounded"
      >
        Get Recommendation
      </button>
      {loading ? (
        <p>Loading...</p>
      ) : (
        recommendation && (
          <p className="mt-4 text-lg font-semibold">Recommendation: {recommendation}</p>
        )
      )}
    </div>
  );
};

export default RecommendationPage;
