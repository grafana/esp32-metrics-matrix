#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include "secrets.h"

// #define DATA_PIN 4
#define RGB_BUILTIN 5
WebServer webServer(80);

void handleRoot() {
  Serial.println("received request to /");
  webServer.send(200, "text/plain", "connected!!");
}

void handleNotFound() {
  webServer.send(404, "text/plain", "Not found");
}

void handlePost() {
  Serial.println("received call to /api/v1/push");
  // jsonDocument.clear();
  if (webServer.hasArg("plain") == false) {
    Serial.println("no arg plain");
  } else {
    Serial.println(webServer.arg("plain"));
  }
  // String body = webServer.arg("plain");
  // deserializeJson(jsonDocument, body);

  // int x_pos = jsonDocument["xPos"];
  // int y_pos = jsonDocument["yPos"];
  // int addr = jsonDocument["addr"];
  // int colorAddr = jsonDocument["colorAddr"];
  // int red_value = jsonDocument["red"];
  // int green_value = jsonDocument["green"];
  // int blue_value = jsonDocument["blue"];

  // if (jsonDocument["addr"] == nullptr){
  //   addr = getAddr(x_pos, y_pos);
  // }

  // if (jsonDocument["colorAddr"] != nullptr){
  //   CRGB color = Wheel(colorAddr);
  //   leds[addr] = color;
  // } else {
  //   leds[addr].setRGB(red_value, green_value, blue_value);
  // }
  // FastLED.show();  
  webServer.send(200, "");
}

void setup()
{
  Serial.begin(115200);

  // pinMode(DATA_PIN, OUTPUT);

  // neopixelWrite(RGB_BUILTIN, RGB_BRIGHTNESS, 0, 0); // red

  WiFi.mode(WIFI_STA);
  Serial.printf("Connecting to %s\n", ssid);

  WiFi.disconnect(true, true);
  WiFi.begin(ssid, pass);

  uint8_t wifiAttempts = 0;
  static uint8_t ledState = 0;

  while (WiFi.status() != WL_CONNECTED && wifiAttempts < 20)
  {
    Serial.print(".");
    ledState = ledState == 0 ? 1 : 0;
    // neopixelWrite(RGB_BUILTIN, ledState == 0 ? RGB_BRIGHTNESS : 0, 0, 0); // Red

    delay(1000);
    if (wifiAttempts == 10)
    {
      WiFi.disconnect(true, true); // Switch off the wifi on making 10 attempts and start again.
      WiFi.begin(ssid, pass);
    }
    wifiAttempts++;
  }
  if (WiFi.status() == WL_CONNECTED)
  {
    // neopixelWrite(RGB_BUILTIN, 0, RGB_BRIGHTNESS, 0); // green
    WiFi.setAutoReconnect(true);                      // Not necessary
    Serial.println();                                 // Not necessary
    Serial.print("Connected with IP: ");              // Not necessary
    Serial.println(WiFi.localIP());                   // Not necessary
  }
  else
  {
    WiFi.disconnect(true, true);
  }
  delay(100);

  webServer.on("/", HTTP_GET, handleRoot);
  webServer.onNotFound(handleNotFound);
  webServer.on("/api/v1/push", HTTP_POST, handlePost);
  webServer.begin();
}

void loop()
{
  // put your main code here, to run repeatedly:
  webServer.handleClient();
}