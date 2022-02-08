# MPU9250-hatire

Arduino code to use the MPU9250 as a head tracker.

## Requirements

https://github.com/hideakitai/MPU9250https://github.com/hideakitai/MPU9250

## Setup

There's some variables in the code that need to be adjusted.

Set the magnetic declination value for your location:
http://www.magnetic-declination.com

Run the `calibrate` code in the MPU9250 library and copy the values to the
corresponding variables.

You can also change the maximum rate of updates.

