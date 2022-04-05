const socket = io();

const distanciaDisplay = document.getElementById('distancia');

socket.on('serial:data', function (dataserial) {
  console.log(dataserial)
  distanciaDisplay.innerHTML = dataserial.Value

  const distanciaIdealId = document.getElementById('distanciaMessage');
  const distanciaContainer = document.getElementById('distancia-container');
  let distanciaIdeal = +localStorage.getItem("distanciaIdeal");
  console.log(distanciaIdeal);

  if (dataserial.Value > distanciaIdeal * 1.3) {
    distanciaIdealId.innerHTML = "Usuário Ausente"
    distanciaContainer.style.visibility = 'hidden';
  } else if (dataserial.Value < distanciaIdeal * 0.5) {
    distanciaIdealId.innerHTML = "Você muito próximo! Você vai ficar cego!"
    distanciaContainer.style.visibility = 'visible'; 
  } else {
    distanciaIdealId.innerHTML = "Você está na da distância ideal"
    distanciaContainer.style.visibility = 'visible'; 
  }

});