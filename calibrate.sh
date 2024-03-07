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
    echo -n -e "c\r" > $TTY
    while [ "$line" != $'START\r' ];
    do
        read line
    done
    while [ "$line" != $'DONE\r' ];
    do
        read line
        echo $line
    done
} < $TTY
