javascript
const socket = new WebSocket('ws://localhost:12345');

socket.onopen = function() {
    console.log('WebSocket Client Connected');
    
    // Пример вызова метода createObject
    const createObjectMessage = {
        jsonrpc: "2.0",
        method: "createObject",
        params: { name: "Object1" },
        id: 1
    };
    socket.send(JSON.stringify(createObjectMessage));

    // Пример вызова метода listObjects
    const listObjectsMessage = {
        jsonrpc: "2.0",
        method: "listObjects",
        params: {},
        id: 2
    };
    socket.send(JSON.stringify(listObjectsMessage));
};

socket.onmessage = function(event) {
    console.log("Received: " + event.data);
};
