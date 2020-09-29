# Flask/Mongo

## Flask API

ruta: 

| Ruta        | Método | Desc.                                | Devuelve             |
| ----------- | ------ | ------------------------------------ | -------------------- |
| /           | GET    | Heath Check                          | "Todo good"          |
| /api/pubs   | GET    | obtener todas las publicaciones      | [{autor,nota},{}...] |
| /api/status | GET    | Obtener RAM, CPU y cantidad de datos | {ram,cpu,len}        |
| /api/add    | POST   | Añadir un nuevo dato                 | {id}                 |
| /api/watch  | GET    | Obtener RAM y CPU                    | {ram,cpu}            |

## Mongo

Database: "galleta"

Collection: "relleno"

## Comandos

### Docker

#### Instalar Docker Engine

`$ sudo apt update`

`$ sudo apt install apt-transport-https ca-certificates curl gnupg-agent software-properties-common`

`$ curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -`

`$ sudo add-apt-repository "deb [arch=amd64] https://download.docker.com/linux/ubuntu $(lsb_release -cs) stable"`

#### Instalar Docker Compose

`$ sudo apt update`

`$ sudo apt install docker-ce docker-ce-cli containerd.io`

`$ sudo curl -L "https://github.com/docker/compose/releases/download/1.26.0/docker-compose-$(uname -s)-$(uname -m)" -o /usr/local/bin/docker-compose`

`$ sudo chmod +x /usr/local/bin/docker-compose`

#### Correr Docker Compose
Build up

`$ sudo docker-compose -f "docker-compose.yml" up -d --build`

Build down

`$ sudo docker-compose -f "docker-compose.yml" down`

### Instalar Git

`$ sudo apt update`

`$ sudo apt install git`

### Modulo Kernel de Linux

#### Instalar Linux-headers

`$ sudo apt update`

`$ sudo apt install linux-headers-$(uname -r)`

`$ sudo apt update && sudo apt upgrade`

`$ sudo apt install build-essential`

#### Instalar modulo

`$ make`

`$ sudo insmod modulo.ko`