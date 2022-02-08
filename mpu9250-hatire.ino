// MIT License
// 
// Copyright (c) 2022 Bernat Arlandis Mañó
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "MPU9250.h" // Use hideakitai library (https://github.com/hideakitai/MPU9250)

// Mag. declination for your country http://www.magnetic-declination.com
// Convert to decimal degrees: degrees + (minutes / 60).
const float magnetic_declination = 0.97;

// Run the calibration code from the MPU9250 library on the Arduino and follow
// the instructions in the serial terminal. Copy the resulting values here:
const float gyro_bias[3] = {-0.78, 0.10, -0.15};
const float mag_bias[3] = {226.56, 232.71, 277.39};
const float mag_scale[3] = {0.96, 1.00, 1.05};

// Maximum update rate
const int rate = 30;

MPU9250 mpu;

// Structure used by Hatire
struct  {
    int16_t  begin;   // Header frame (2 bytes)
    uint16_t cpt;      // Frame number (2 bytes)
    float    gyro[3];   // Gyroscope [Y, P, R] (12 bytes)
    float    acc[3];    // Accelerometer [x, y, z] (12 bytes)
    int16_t  end;      // Footer frame (2 bytes)
} hat;

void setup()
{
    // Init serial port
    Serial.begin(115200);

    // Join the I2C bus as master
    Wire.begin();
    delay(2000);

    // Setup MPU9250
    mpu.setup(0x68);
    mpu.setMagneticDeclination(magnetic_declination);
    mpu.setGyroBias(gyro_bias[0], gyro_bias[1], gyro_bias[2]);
    mpu.setMagBias(mag_bias[0], mag_bias[1], mag_bias[2]);
    mpu.setMagScale(mag_scale[0], mag_scale[1], mag_scale[2]);

    // Init Hatire struct
    hat.begin = 0xAAAA;
    hat.cpt = 0;
    hat.end = 0x5555;

    // Wait for serial port ready
    while (!Serial);
}

void loop()
{
    if (mpu.update()) {
        static uint32_t last = millis();
        uint32_t now = millis();
        if ((now - last - 1000 / rate) >= 0) {
            hat.gyro[0] = mpu.getYaw();
            hat.gyro[1] = mpu.getPitch();
            hat.gyro[2] = mpu.getRoll();

            Serial.write((byte*)&hat, sizeof(hat));

            hat.cpt++;
            if (hat.cpt > 999) {
                hat.cpt = 0;
            }

            last = now;
        }
    }
}
