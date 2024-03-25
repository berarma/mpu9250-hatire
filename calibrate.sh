#!/bin/bash

TTY=/dev/ttyACM0

if [ "$1" != "" ];
then
    TTY="$1"
fi

if [ ! -c "$TTY" ];
then
    echo "Syntax: $0 [ttydev]"
    echo "Error: Device '$TTY' not found"
    exit 1
fi

echo "Sending calibration request..."
echo -e "Device response:\n"

{
    echo -ne "c\r" > $TTY
    while [ "$line" != "START" ];
    do
        read line
    done
    while [ "$line" != "DONE" ];
    do
        read line
        echo $line
    done
} < $TTY
