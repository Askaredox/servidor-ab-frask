#!/bin/sh

# Antes de correr
## chmod +x servidor.sh

# instalar Docker Engine
sudo apt update
sudo apt install -y apt-transport-https ca-certificates curl gnupg-agent software-properties-common
curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -
sudo add-apt-repository "deb [arch=amd64] https://download.docker.com/linux/ubuntu $(lsb_release -cs) stable"

# instalar Docker Compose
sudo apt update
sudo apt install -y docker-ce docker-ce-cli containerd.io
sudo curl -L "https://github.com/docker/compose/releases/download/1.26.0/docker-compose-$(uname -s)-$(uname -m)" -o /usr/local/bin/docker-compose
sudo chmod +x /usr/local/bin/docker-compose

# instalar Git
sudo apt update
sudo apt install -y git

# instalar Modulo Kernel de Linux
sudo apt update
sudo apt install -y linux-headers-$(uname -r)
sudo apt update && sudo apt upgrade -y
sudo apt install -y build-essential

# preparar
git clone https://github.com/Askaredox/servidor-ab-frask.git
cd servidor-ab-frask/servab/modulo
make
sudo insmod modulo.ko
cd ../..
sudo docker-compose -f "docker-compose.yaml" up -d --build
sudo docker ps
echo Ready!

# Extras

## Bajar de Git
### git pull origin master

## Preparar Docker
### sudo docker-compose -f "docker-compose.yaml" up -d --build

## Desconectar Docker
### sudo docker-compose -f "docker-compose.yaml" down

## Instalar modulo
### sudo insmod modulo.ko

## Desinstalar modulo
### sudo rmmod modulo