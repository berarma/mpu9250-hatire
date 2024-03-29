// MIT License
// 
// Copyright (c) 2018 Hideaki Tai
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

#include <EEPROM.h>
#include "MPU9250.h" // Use hideakitai library (https://github.com/hideakitai/MPU9250)

#define DEBUG true
#define EEPROM_OFFSET 0
#define UPDATE_RATE 100

MPU9250 mpu;

// Structure used by Hatire
struct  {
    int16_t  begin;   // Header frame (2 bytes)
    uint16_t cpt;      // Frame number (2 bytes)
    float    gyro[3];   // Gyroscope [Y, P, R] (12 bytes)
    float    acc[3];    // Accelerometer [x, y, z] (12 bytes)
    int16_t  end;      // Footer frame (2 bytes)
} hat;

const uint8_t EEPROM_SIZE = 1 + sizeof(float) * 3 * 4;

enum EEP_ADDR {
    EEP_CALIB_FLAG = EEPROM_OFFSET + 0x00,
    EEP_ACC_BIAS   = EEPROM_OFFSET + 0x01,
    EEP_GYRO_BIAS  = EEPROM_OFFSET + 0x0D,
    EEP_MAG_BIAS   = EEPROM_OFFSET + 0x19,
    EEP_MAG_SCALE  = EEPROM_OFFSET + 0x25
};

byte readByte(int address) {
    byte valueIn = 0;
    EEPROM.get(address, valueIn);
    return valueIn;
}

float readFloat(int address) {
    float valueIn = 0;
    EEPROM.get(address, valueIn);
    return valueIn;
}

bool isCalibrated() {
    return (readByte(EEP_CALIB_FLAG) == 0x01);
}

void saveCalibration() {
    EEPROM.put(EEP_CALIB_FLAG, 1);
    EEPROM.put(EEP_ACC_BIAS + 0, mpu.getAccBias(0));
    EEPROM.put(EEP_ACC_BIAS + 4, mpu.getAccBias(1));
    EEPROM.put(EEP_ACC_BIAS + 8, mpu.getAccBias(2));
    EEPROM.put(EEP_GYRO_BIAS + 0, mpu.getGyroBias(0));
    EEPROM.put(EEP_GYRO_BIAS + 4, mpu.getGyroBias(1));
    EEPROM.put(EEP_GYRO_BIAS + 8, mpu.getGyroBias(2));
    EEPROM.put(EEP_MAG_BIAS + 0, mpu.getMagBias(0));
    EEPROM.put(EEP_MAG_BIAS + 4, mpu.getMagBias(1));
    EEPROM.put(EEP_MAG_BIAS + 8, mpu.getMagBias(2));
    EEPROM.put(EEP_MAG_SCALE + 0, mpu.getMagScale(0));
    EEPROM.put(EEP_MAG_SCALE + 4, mpu.getMagScale(1));
    EEPROM.put(EEP_MAG_SCALE + 8, mpu.getMagScale(2));
#if defined(ESP_PLATFORM) || defined(ESP8266)
    EEPROM.commit();
#endif
}

void loadCalibration() {
    if (isCalibrated()) {
        mpu.setAccBias(
            readFloat(EEP_ACC_BIAS + 0),
            readFloat(EEP_ACC_BIAS + 4),
            readFloat(EEP_ACC_BIAS + 8));
        mpu.setGyroBias(
            readFloat(EEP_GYRO_BIAS + 0),
            readFloat(EEP_GYRO_BIAS + 4),
            readFloat(EEP_GYRO_BIAS + 8));
        mpu.setMagBias(
            readFloat(EEP_MAG_BIAS + 0),
            readFloat(EEP_MAG_BIAS + 4),
            readFloat(EEP_MAG_BIAS + 8));
        mpu.setMagScale(
            readFloat(EEP_MAG_SCALE + 0),
            readFloat(EEP_MAG_SCALE + 4),
            readFloat(EEP_MAG_SCALE + 8));
    }
}

#if DEBUG
void print_calibration()
{
    Serial.println("< calibration parameters >");
    Serial.print(mpu.getAccBiasX() * 1000.f / (float)MPU9250::CALIB_ACCEL_SENSITIVITY);
    Serial.print(", ");
    Serial.print(mpu.getAccBiasY() * 1000.f / (float)MPU9250::CALIB_ACCEL_SENSITIVITY);
    Serial.print(", ");
    Serial.print(mpu.getAccBiasZ() * 1000.f / (float)MPU9250::CALIB_ACCEL_SENSITIVITY);
    Serial.println();
    Serial.print(mpu.getGyroBiasX() / (float)MPU9250::CALIB_GYRO_SENSITIVITY);
    Serial.print(", ");
    Serial.print(mpu.getGyroBiasY() / (float)MPU9250::CALIB_GYRO_SENSITIVITY);
    Serial.print(", ");
    Serial.print(mpu.getGyroBiasZ() / (float)MPU9250::CALIB_GYRO_SENSITIVITY);
    Serial.println();
    Serial.print(mpu.getMagBiasX());
    Serial.print(", ");
    Serial.print(mpu.getMagBiasY());
    Serial.print(", ");
    Serial.print(mpu.getMagBiasZ());
    Serial.println();
    Serial.print(mpu.getMagScaleX());
    Serial.print(", ");
    Serial.print(mpu.getMagScaleY());
    Serial.print(", ");
    Serial.print(mpu.getMagScaleZ());
    Serial.println();
}
#endif

void setup()
{
    // Init serial port
    Serial.begin(115200);

    // Join the I2C bus as master
    Wire.begin();
    delay(2000);

    // Setup MPU9250
    mpu.setup(0x68);

#if defined(ESP_PLATFORM) || defined(ESP8266)
    EEPROM.begin(0x80);
#endif

    // load from eeprom
    loadCalibration();

    // Init Hatire struct
    hat.begin = 0xAAAA;
    hat.cpt = 0;
    hat.end = 0x5555;

    // Wait for serial port ready
    while (!Serial);

#if DEBUG
    print_calibration();
#endif
}

void calibrate()
{
    Serial.println();
    Serial.println("START");
    Serial.println("Please leave the device still.");
    delay(2000);
    mpu.calibrateAccelGyro();

    Serial.println("Please wave device in a figure eight until done.");
    delay(2000);
    mpu.calibrateMag();

#if DEBUG
    print_calibration();
#endif

    Serial.println("DONE");

    // save to eeprom
    saveCalibration();
}

void loop()
{
    if (Serial.available()) {
        char input;
        input = Serial.read();
        if (input == 'c') {
            calibrate();
        }
    }

    if (mpu.update()) {
        static uint32_t last = millis();
        uint32_t now = millis();
        if ((now - last - 1000 / UPDATE_RATE) >= 0) {
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
