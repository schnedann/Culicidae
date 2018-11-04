#!/bin/bash
if(test -d mosquitto)
then
cd mosquitto
pwd
git pull
cd ..
else
git clone https://github.com/schnedann/mosquitto.git
fi
pwd