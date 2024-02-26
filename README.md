# MPU9250-hatire

Arduino code to use the MPU9250 as a head tracker.

## Hardware

Tested with Arduino Pro Micro (5V) and MPU9250 (5V).

Both the MPU9250 and Arduino Pro Micro have 3.3V and 5V versions. Use versions
with matching voltages.

Required connections:

|Arduino |MPU9250 |
|--------|--------|
|VCC     |VCC     |
|GND     |GND     |
|SDA     |SDA     |
|SCL     |SCL     |
|GND     |AD0     |

In the Arduino Pro Micro, SDA is pin 2 and SCL is pin 3.

## Software

- [MPU9250 hideaktai's library](https://github.com/hideakitai/MPU9250)

## Setup

Upload the code to your Arduino and calibrate your device. Calibration needs to
be done at least once and when the sensor starts giving bad readings. Changes
in the environmental magnetism may require a calibration.

At the top of the source code in `mpu9250-hatire.ino`, there are some constants
which can be changed to adjust the sketch, although it's not required.

`DEBUG`: Change to `false` to save around 400 bytes of program space. In case
your Arduino doesn't have enough program memory.

`EEPROM_OFFSET`: Start address to store the calibration values. For devices
with worn-out EEPROMs or if you want to manually level wear.

`UPDATE_RATE`: Maximum update rate to send sensor data to the computer.

## Calibration

The calibration process can be started by sending a "c" character to the
serial port. The device will answer with instructions.

A Bash script called `calibrate.sh` is included to trigger the calibration
process. Run with the device as parameter (optional) and follow the
instructions in the terminal.

## Contributing

Please, use the issues to discuss bugs, ideas, etc.

## Disclaimer

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
