#include "telaire_T9602.h"

// Constructor
T9602::T9602() {
    pinMode(LED_BUILTIN, OUTPUT); // Initialize the LED pin as an output
    sensorAddress = 0x28; // Default T9602 sensor I2C address
    relativeHumidity = -9999;
    temperature = -9999;
}

void T9602::begin(uint8_t address) {
    sensorAddress = address;
    Wire.begin();
}

void T9602::updateMeasurements() {
    // Turn the LED on by making the voltage LOW
    digitalWrite(LED_BUILTIN, LOW);

    uint8_t data[4] = {0};
    Wire.beginTransmission(sensorAddress);
    Wire.write(0x00); // Command to read data
    Wire.endTransmission();
    Wire.requestFrom(sensorAddress, 4); // Request 4 bytes of data

    // Store the 4 bytes of data
    for (int i = 0; i < 4; ++i) {
        data[i] = Wire.read();
    }

    // Calculate relative humidity
    relativeHumidity = static_cast<float>(((data[0] & 0x3F) << 8) + data[1]) / 16384.0 * 100.0;
    // Calculate temperature
    temperature = static_cast<float>((static_cast<unsigned>(data[2]) * 64 + (static_cast<unsigned>(data[3]) >> 2)) / 16384.0 * 165.0 - 40.0);

    // Turn the LED off by making the voltage HIGH
    digitalWrite(LED_BUILTIN, HIGH);
}

float T9602::getHumidity() {
    return relativeHumidity;
}

float T9602::getTemperature() {
    return temperature;
}

String T9602::getString(bool takeNewReadings) {
    if (takeNewReadings) {
        updateMeasurements();
    }

    // Adjusted format to "Temp: value; Humidity: value"
    return "Temp: " + String(temperature) + "; Humidity: " + String(relativeHumidity);
}

String T9602::getHeader() {
    return "Relative Humidity [%],Temp [C],";
}
