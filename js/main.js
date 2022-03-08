const socket = io();

const distanciaDisplay = document.getElementById('distancia');

socket.on('serial:data', function (dataserial) {
  console.log(dataserial);
});