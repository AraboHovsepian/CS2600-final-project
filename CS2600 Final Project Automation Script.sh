#!/bin/bash

while true;
do
mosquitto_pub -t "esp32/Receive" -m "1"
mosquitto_sub -t 'esp32/send' -C 1
mosquitto_pub -t 'esp32/Receive' -m '0'
mosquitto_sub -t 'esp32/send' -C 1
done
