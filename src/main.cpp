#include <Arduino.h>
#include <WiFi.h>
// #include <WebServer.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "secrets.h"

#include <WriteRequest.h>


#define PRINT(...)                  \
    {                               \
      Serial.print(__VA_ARGS__);    \
    }
#define PRINTLN(...)                \
    {                               \
      Serial.println(__VA_ARGS__);  \
    }

#define HEAP()                      \
    {                               \
      Serial.print("Free Heap: ");  \
      Serial.println(freeMemory()); \
    }

// #define DATA_PIN 4
#define RGB_BUILTIN 5
// WebServer webServer(80);
AsyncWebServer server(80);

// void handleRoot() {
//   Serial.println("received request to /");
//   webServer.send(200, "text/plain", "connected!!");
// }

// void handleNotFound() {
//   webServer.send(404, "text/plain", "Not found");
// }

WriteRequest writeRequest(1, 2048);

QueueHandle_t queue;
SemaphoreHandle_t  bufferMtx;
uint8_t buffer[2048];
uint8_t queueItem = 1;


void notFound(AsyncWebServerRequest* request) {
  request->send(404, "text/plain", "Not found");
}

void handlePost(AsyncWebServerRequest* request, uint8_t* data, size_t len, size_t index, size_t total) {
  // PRINT("Starting heap: ");
  // HEAP();
  // Serial.print("Stack high water mark: ");
  // Serial.println(uxTaskGetStackHighWaterMark(NULL));

  Serial.println("received call to /api/v1/push");
  Serial.println(xPortGetCoreID());
  Serial.print("len: ");
  Serial.print(len);
  Serial.print(" index: ");
  Serial.print(index);
  Serial.print(" total: ");
  Serial.println(total);

  if (len != total) {
    Serial.println("len != total, we can't handle big packets yet");
    return;
  }

  if (len > sizeof(buffer)) {
    Serial.println("len > sizeof(buffer)");
    return;
  }

  if (xSemaphoreTake(bufferMtx, 10) != pdTRUE) {
    Serial.println("Timed out taking bufferMtx, skipping this message");
    return;
  }
  if (xQueueSend(queue, &len, 0) != pdTRUE) {
    // I don't think this is possible to end up here, but just in case.
    Serial.println("had the mutex but the queue was full");
    xSemaphoreGive(bufferMtx);
    return;
  }
  // Copy the data into the buffer
  memcpy(buffer, data, len);
  xSemaphoreGive(bufferMtx);



  // if (webServer.hasArg("plain") == false) {
  //   Serial.println("no arg plain");
  //   return;
  // }

  // String body = webServer.arg("plain");
  // const char* bcstr = body.c_str();
  // // Serial.println(strlen(bcstr));
  // Serial.println();
  // for (int i = 0; i < strlen(bcstr); i++) {
  //   Serial.print(bcstr[i], HEX);
  //   Serial.print(" ");
  // }
  // Serial.println();













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
  // webServer.send(200, "");
  // PRINT("Ending heap: ");
  // HEAP();
}

void processTask(void* parameters) {
  uint8_t len;
  for (;;) {
    // Serial.println("waiting for message");
    // Serial.println(xPortGetCoreID());
    vTaskDelay(10);
    if (xQueueReceive(queue, &len, 5) != pdTRUE) {
      // Serial.println("Timed out waiting for queue message");
      continue;
    }
    Serial.println("locking mutex");
    if (xSemaphoreTake(bufferMtx, 100) != pdTRUE) {
      // Serial.println("Timed out waiting for mutex");
      continue;
    }
    Serial.println("deserializing");
    uint64_t start = millis();
    writeRequest.fromSnappyProto(buffer, len);
    Serial.print("deserialized in ");
    Serial.print(millis() - start);
    Serial.println("ms");
    Serial.println("returning mutex");
    xSemaphoreGive(bufferMtx);
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.println(xPortGetCoreID());
  Serial.print("Stack high water mark: ");
  Serial.println(uxTaskGetStackHighWaterMark(NULL));

  queue = xQueueCreate(1, sizeof(uint8_t));
  bufferMtx = xSemaphoreCreateMutex();

  if (queue == NULL) {
    Serial.println("Error creating the queue");
  }

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

  writeRequest.setDebug(Serial);

  // webServer.on("/", HTTP_GET, handleRoot);
  // webServer.onNotFound(handleNotFound);
  // webServer.on("/api/v1/push", HTTP_POST, handlePost);
  // webServer.begin();
  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(200, "text/plain", "Hello, world");
    });

  server.onNotFound(notFound);
  server.on("/api/v1/push", HTTP_POST, [](AsyncWebServerRequest* request) {
    request->send(200, "text/plain", "");
    }, NULL, handlePost);
  server.begin();
  Serial.print("Stack high water mark: ");
  Serial.println(uxTaskGetStackHighWaterMark(NULL));

  // TODO I don't know that this is needed anymore, we could maybe just put it back in loop()
  // Snappy decompression needs a bigger stack, so we create a new task where we can define a bigger stack.
  // xTaskCreate(processTask, "ProcessTask", 32768, NULL, 1, NULL);
  xTaskCreatePinnedToCore(processTask, "ProcessTask", 8192, NULL, 1, NULL, 1);

}



void loop()
{
  // uint8_t len;
  // if (xQueueReceive(queue, &len, portMAX_DELAY) != pdTRUE) {
  //   Serial.println("Timed out waiting for queue message");
  //   return;
  // }
  // if (xSemaphoreTake(bufferMtx, 100) != pdTRUE) {
  //   Serial.println("Timed out waiting for mutex");
  //   return;
  // }
  // writeRequest.fromSnappyProto(buffer, len);
  // xSemaphoreGive(bufferMtx);




  // put your main code here, to run repeatedly:
  // webServer.handleClient();
  vTaskDelay(10);
}