const http = require('http');
const express = require('express');
const SocketIO = require('socket.io');

const app = express();
const server = http.createServer(app);
const { Server } = require("socket.io");
const io = new Server(server);

app.use(express.static('public')); 

app.get('/', (req, res, next) =>{
    res.sendFile(__dirname + '/public/index.html')
});

io.on('connection', (socket) => {
    console.log('a user connected');
});

server.listen(5000, () => {
    console.log('server on port 5000');
});

//////////////////////////////////////////////////////////////////////////
const { SerialPort } = require('serialport');
const { ReadlineParser } = require('@serialport/parser-readline');

const port = new SerialPort({
    // path: 'COM5', 
    path: '/dev/ttyACM0', 
    baudRate: 9600 
});

const parser = port.pipe(new ReadlineParser({ delimiter: '\r\n' }));
parser.on('data', function(data){
    console.log(data);
    io.emit('serial:data', {
        Value: data.toString()
    });
});