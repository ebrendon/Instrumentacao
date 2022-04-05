# Projeto de Instrumentação - Medição de Distância com HC-SR04

Trabalho desenvolvido na disciplinas de Instrumentação para medição de distância utilizando arduino e sensores, aplicado a utilização ergonômica da estação de trabalho/estudo.


## Instalação

Deve-se ter o node.js instalado na máquina.
Instalar as dependências necessárias:
- serialport
- express
- socket.io

*Obs: Baixando os arquivos package.json e package-lock.json é possível instalar todas as dependências apenas com o comando npm install*

## Execução

- Em app.js alterar o path com a porta utilizada pelo arduino na sua máquina
- Em app.js definir a porta na linha 20, por padrão o código define a porta 5000
- No terminal executar 'node app.js'
- No navegador digitar 'localhost:(porta definida)'


## Em desenvolvimento:
1. Firmware
- [x] Detecção de presença com MaxSonar Ez1
- [x] Medição de distância com HC-SR04
  
2. Servidor web
- [x] Comunicação entre Arduino e Servidor (Back-end)
- [x] Exibição dos dados no navegador (Back-end)
- [x] Protótipo do Site (Front-end)
- [x] Estilização (Front-end)
- [x] Timer (Back-end)
- [x] Coleta dos dados inseridos pelo usuário

## Projeto Finalizado

