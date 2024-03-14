# T9602 Sensor MQTT Publisher

This project publishes Telaire T9602 temperature and humidity sensor data to MQTT topics, making it suitable for IoT applications.
It's designed to run on ESP8266/ESP32 modules and integrates with any standard MQTT broker for easy data integration with home automation systems, data logging, or any custom IoT application.

## Features

- Reads temperature and humidity data from the Telaire T9602 sensor.
- Publishes sensor data to MQTT topics at regular intervals.
- Configurable MQTT and WiFi settings.

## Getting Started
On Wemos D1 mini use i2c pins (D1(GPIO 5)-SCL,D2(GPIO 4)-SDA) to connect the T9602 sensor.

[Amphenol Telaire T9602](https://www.amphenol-sensors.com/en/telaire/humidity/527-humidity-sensors/3224-t9602)

[T9602 Datasheet](https://www.farnell.com/datasheets/1872050.pdf)

##  Sensor specifications

  * IP67 rated sensor with two sealing membranes
  * -20 to 70 °C temperature range
  * Accuracy: ±2% RH, 0-95% RH ±0.5°C
  * 14 bit ADC resolution

## Sensor cable pinout

| **Color** |   **Signal**   |
|-----------|----------------|
| Red       | V+             |
| Green     | GND            |
| White     | SDA            |
| Black     | SCL            |

![T9602 pinout](https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcS4IXIdSB64S-NKB0MQdY745BLXDTEJgOI-VA&usqp=CAU)

[Internally used ChipCap2 sensor module reference guide](https://www.amphenol-sensors.com/hubfs/Documents/AAS-916-127J-Telaire-ChipCap2-022118-web.pdf?Itemid=8487+%27)

### Prerequisites

- An ESP8266 module (e.g., Wemos D1 Mini, NodeMCU).
- A Telaire T9602 temperature and humidity sensor.
- Arduino IDE for code editing and ESP8266 programming.
- Access to a WiFi network and MQTT broker.

### Installation

1. Clone this repository to your local machine or download the ZIP file and extract it.
2. Open the project in the Arduino IDE.
3. Install the `ESP8266WiFi` and `PubSubClient` libraries via the Arduino Library Manager.
4. Update the `ssid`, `password`, `mqttServer`, `mqttPort`, `mqttUser`, and `mqttPassword` in the code with your WiFi and MQTT broker credentials.
5. Connect the T9602 sensor to your ESP8266 module according to the sensor's datasheet.
6. Upload the code to your ESP8266 module.

## Usage

Once powered, the it will connect to your WiFi network and establish a connection to the MQTT broker. 
It will then periodically read temperature and humidity data from the T9602 sensor and publish it to `/T9602/temp` and `/T9602/humidity` MQTT topics.

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details.