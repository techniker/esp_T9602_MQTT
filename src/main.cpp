/*
 * Implementation of the MQTT publishing functionality for
 * Telaire T9602 temperature and humidity sensor data and displaying it on SSD1306 OLED. It includes WiFi and MQTT
 * setup, sensor data acquisition, and publishing to specified MQTT topics.
 *
 * Author: Bjoern Heller <tec att sixtopia.net>
 */

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "telaire_T9602.h"
#include "telaire_T9602.cpp"
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3c ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// WiFi Network Settings
const char* ssid = "";
const char* password = "";

// MQTT Broker Settings
const char* mqttServer = "";
const int mqttPort = 1883;
const char* mqttUser = "";
const char* mqttPassword = "";

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
        if (mqttClient.connect("Sensor-Node01", mqttUser, mqttPassword)) {
            Serial.println("Connected");
        } else {
            Serial.print("Failed, rc=");
            Serial.print(mqttClient.state());
            Serial.println(" Trying again in 5 seconds.");
            delay(5000);
        }
    }
}

void welcomeOLED(void) {
  display.clearDisplay();
  display.setTextSize(4); // Draw 4X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 0);
  display.println(F("NODE01"));
  display.display(); // Show text
  delay(100);

  // Scroll in various directions, pausing in-between:
  display.startscrollright(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(500);
  display.startscrollleft(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
}

void displaySensorValues(float temperature, float humidity) {
    if (digitalRead(D8) == HIGH) {
        // D8 is HIGH, update the display
        display.ssd1306_command(SSD1306_DISPLAYON); // Turn off the display
        display.clearDisplay();
        display.setTextSize(2); // Larger text size for readability
        display.setTextColor(SSD1306_WHITE);

        display.setCursor(0, 0); // Position for temperature
        display.print("T:");
        display.print(temperature, 2);
        display.print(" C");

        display.setCursor(0, 32); // Position for humidity
        display.print("H:");
        display.print(humidity, 2);
        display.print(" %");

        display.display(); // Apply updates to the display
    } else {
        // Optional: Clear the display or put the display in a low-power state
        // if D8 is not HIGH. This part is up to your specific needs.
        display.ssd1306_command(SSD1306_DISPLAYOFF); // Turn off the display
    }
}


void setup() {
    pinMode(D8, INPUT_PULLUP);
    Serial.begin(115200);
    setupWiFi();
    mqttClient.setServer(mqttServer, mqttPort);
    telaireSensor.begin();

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
    }
    display.display();
    display.clearDisplay();
    display.startscrollright(0x00, 0x0F);
    welcomeOLED();
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

    mqttClient.publish("/T9602-1/temp", temperatureString);
    delay(10);
    mqttClient.publish("/T9602-1/humidity", humidityString);

    Serial.print("Temp: ");
    Serial.print(temperatureString);
    Serial.print("; Humidity: ");
    Serial.println(humidityString);

    // Call the new function to display temperature and humidity
    displaySensorValues(temperature, humidity);
    
    // Toggle LED to indicate data publish
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
    digitalWrite(LED_BUILTIN, HIGH);

    delay(1000); // Wait a bit before the next loop iteration
}
