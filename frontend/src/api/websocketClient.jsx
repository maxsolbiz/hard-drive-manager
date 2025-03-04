class WebSocketClient {
  constructor(url) {
    this.socket = new WebSocket(url);
    this.listeners = [];

    this.socket.onmessage = (event) => {
      try {
        const data = JSON.parse(event.data);
        this.listeners.forEach((callback) => callback(data));
      } catch (e) {
        console.error('Error parsing WebSocket message:', e);
      }
    };

    this.socket.onerror = (error) => {
      console.error('WebSocket error:', error);
    };
  }

  subscribe(callback) {
    this.listeners.push(callback);
  }

  unsubscribe(callback) {
    this.listeners = this.listeners.filter((cb) => cb !== callback);
  }
}

// Use the environment variable; update port if needed.
export default new WebSocketClient(
  process.env.REACT_APP_WEBSOCKET_URL || 'ws://localhost:9002'
);
