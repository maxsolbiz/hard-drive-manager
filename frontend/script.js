// frontend/script.js
const statusElement = document.getElementById('status');
const messagesElement = document.getElementById('messages');

const socket = new WebSocket('ws://localhost:9002');

socket.onopen = () => {
    statusElement.textContent = 'Connected to WebSocket server';
    socket.send('Hello from frontend!');
};

socket.onmessage = (event) => {
    const message = `Message from server: ${event.data}\n`;
    messagesElement.textContent += message;
};

socket.onerror = (error) => {
    statusElement.textContent = 'WebSocket error';
    console.error('WebSocket error:', error);
};

socket.onclose = () => {
    statusElement.textContent = 'WebSocket connection closed';
};