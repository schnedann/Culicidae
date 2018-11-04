#!/bin/bash
./get_mosquitto.sh
if(test -d mosquitto)
then
cd mosquitto
pwd
make clean && make binary
cd ..
fi
pwd