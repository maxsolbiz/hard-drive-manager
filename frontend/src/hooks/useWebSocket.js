import { useState, useEffect } from 'react';

export default function useWebSocket(url) {
  const [socketData, setSocketData] = useState(null);
  const [status, setStatus] = useState('disconnected');

  useEffect(() => {
    const ws = new WebSocket(url);

    ws.onopen = () => {
      console.log(`Connected to ${url}`);
      setStatus('connected');
    };

    ws.onmessage = (event) => {
      // Parse JSON data if applicable
      try {
        const data = JSON.parse(event.data);
        setSocketData(data);
      } catch (err) {
        setSocketData(event.data);
      }
    };

    ws.onerror = (error) => {
      console.error('WebSocket error:', error);
    };

    ws.onclose = () => {
      console.log('WebSocket disconnected');
      setStatus('disconnected');
    };

    return () => {
      ws.close();
    };
  }, [url]);

  return { socketData, status };
}
