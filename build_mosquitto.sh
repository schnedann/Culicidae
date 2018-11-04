#!/bin/bash
./get_mosquitto
if(test -d mosquitto)
then
cd mosquitto
pwd
make clean && make binary
cd ..
fi
pwd