#!/bin/sh

cd servidor-ab-frask/servab/modulo
make clean
sudo rmmod modulo
make
sudo insmod modulo.ko
cd ../..
sudo docker-compose -f "docker-compose.yaml" up -d --build
sudo docker ps
echo Ready!