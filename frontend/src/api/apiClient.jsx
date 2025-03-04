import axios from 'axios';

const apiClient = axios.create({
  baseURL: process.env.REACT_APP_BACKEND_URL, // e.g., http://localhost:8000
  timeout: 30000,
});

export const fetchDrives = async () => {
  try {
    const response = await apiClient.get('/drives');
    return response.data;
  } catch (error) {
    console.error("Error fetching drives:", error);
    throw error;
  }
};

export const fetchDriveHealth = async (driveName) => {
  try {
    const response = await axios.get(`${process.env.REACT_APP_BACKEND_URL}/health/${driveName}`);
    console.log("Fetched drive health for", driveName, ":", response.data);
    return response.data;
  } catch (error) {
    console.error("Error fetching drive health:", error);
    throw error;
  }
};

export default apiClient;
