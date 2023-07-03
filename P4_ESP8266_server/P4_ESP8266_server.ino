// Import required libraries
#include <ESP8266WiFi.h>
#include "ESPAsyncWebServer.h"

// Set your access point network credentials
const char* ssid = "ESP8266-Access-Point";
const char* password = "123456789";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

const int flexPin = A0;			// Pin connected to voltage divider output

const float VCC = 3.3;			// voltage at ESP 3V line
const float R_DIV = 10000.0;	// resistor used to create a voltage divider
const float flatResistance = 25000.0;	// resistance when flat
const float bendResistance = 100000.0;	// resistance at 90 deg

String readSensor() {
  // Read the ADC, and calculate voltage and resistance from it
	int ADCflex = analogRead(flexPin);
	float Vflex = ADCflex * VCC / 1023.0;
	float Rflex = R_DIV * (VCC / Vflex - 1.0);

	// Use the calculated resistance to estimate the sensor's bend angle:
	float angle = map(Rflex, flatResistance, bendResistance, 0, 90.0);

  if (String(angle) == "0.00") {
    return "Good Posture";
  } else {
    return "Bad Posture";
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println();

	pinMode(flexPin, INPUT);

   // Setting the ESP as an access point
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.on("/sensor", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send_P(200, "text/plain", readSensor().c_str());
  });

  // Start server
  server.begin();
}

void loop() {
}