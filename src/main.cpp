#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include "./config.h"


WiFiClient client;
HTTPClient httpClient;

#define DETECTION_PIN D0

void sendPost(const char* url) {
	httpClient.begin(client, url);
	httpClient.GET();
	String content = httpClient.getString();
	httpClient.end();

	Serial.println(content);
}

void setup()
{
	Serial.begin(9600);
	Serial.println("Booting");
	pinMode(DETECTION_PIN,INPUT_PULLDOWN_16);

	WiFi.begin(SECRET_SSID, SECRET_PASS);
	int totalConnectingDelay = 0;
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
		totalConnectingDelay += 500;
		if (totalConnectingDelay > 15000) {
			ESP.restart();
		}
	}

	Serial.println("Connected");
}

void loop()
{
	int detectionState = digitalRead(DETECTION_PIN);

	if (detectionState == HIGH) {
		sendPost(URL_ALARM);
	}

	sendPost(URL_MONITOR);
	delay(500);
}