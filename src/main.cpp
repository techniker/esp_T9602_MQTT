/*
 * Implementation of the MQTT publishing functionality for
 * Telaire T9602 temperature and humidity sensor data. It includes WiFi and MQTT
 * setup, sensor data acquisition, and publishing to specified MQTT topics.
 *
 * Author: Bjoern Heller <tec att sixtopia.net>
 */

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "telaire_T9602.h"
#include <Arduino.h>
#include <Wire.h>

// WiFi Network Settings
const char* ssid = "yourSSID";
const char* password = "yourPASSWORD";

// MQTT Broker Settings
const char* mqttServer = "yourMQTTBrokerAddress";
const int mqttPort = 1883;
const char* mqttUser = "yourMQTTUser";
const char* mqttPassword = "yourMQTTPassword";

WiFiClient espClient;
PubSubClient mqttClient(espClient);
T9602 telaireSensor;

void setupWiFi() {
    delay(10);
    Serial.println("\nConnecting to WiFi...");
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nWiFi Connected.");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
}

void reconnectMQTT() {
    while (!mqttClient.connected()) {
        Serial.print("Attempting MQTT connection...");
        if (mqttClient.connect("ESP8266Client", mqttUser, mqttPassword)) {
            Serial.println("Connected");
        } else {
            Serial.print("Failed, rc=");
            Serial.print(mqttClient.state());
            Serial.println(" Trying again in 5 seconds.");
            delay(5000);
        }
    }
}

void setup() {
    Serial.begin(115200);
    setupWiFi();
    mqttClient.setServer(mqttServer, mqttPort);
    telaireSensor.begin();
}

void loop() {
    if (!mqttClient.connected()) {
        reconnectMQTT();
    }
    mqttClient.loop();

    telaireSensor.updateMeasurements();
    float temperature = telaireSensor.getTemperature();
    float humidity = telaireSensor.getHumidity();

    char temperatureString[8];
    dtostrf(temperature, 6, 2, temperatureString);
    char humidityString[8];
    dtostrf(humidity, 6, 2, humidityString);

    mqttClient.publish("/T9602/temp", temperatureString);
    mqttClient.publish("/T9602/humidity", humidityString);

    Serial.print("Temp: ");
    Serial.print(temperatureString);
    Serial.print("; Humidity: ");
    Serial.println(humidityString);

    // Toggle LED to indicate data publish
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
    digitalWrite(LED_BUILTIN, HIGH);

    delay(1000); // Wait a bit before the next loop iteration
}
