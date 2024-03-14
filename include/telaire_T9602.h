#ifndef TELAIRE_T9602_H
#define TELAIRE_T9602_H

#include <Arduino.h>
#include <Wire.h>

class T9602 {
public:
    T9602();
    void begin(uint8_t address = 0x28); // Default sensor I2C address set as optional parameter
    void updateMeasurements(); // Fetches new measurement data from the sensor
    float getHumidity(); // Returns the last measured humidity value
    float getTemperature(); // Returns the last measured temperature value
    String getString(bool takeNewReadings = false); // Returns formatted string with temperature and humidity
    String getHeader(); // Returns CSV header string

private:
    uint8_t sensorAddress; // I2C address of the T9602 sensor
    float relativeHumidity; // Last measured relative humidity
    float temperature; // Last measured temperature
};

#endif // TELAIRE_T9602_H
