#include <Arduino.h>
#include <WiFi.h>
#include <Preferences.h>

#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include "certificates.h"
#include "secrets.h"

#include <PromLokiTransport.h>
#include <PrometheusArduino.h>
#include "webserver.h"

PromLokiTransport transport;
PromClient client(transport);


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

Preferences preferences;

AsyncWebServer server(80);

WriteRequest writeRequest(1, 4096);
TimeSeries ts = TimeSeries(1, 20, 20, 100, 5);

ReadRequest readRequest(1, 2048);
TimeSeries querySeries = TimeSeries(1, 20, 20, 50, 5);

QueueHandle_t queue;
SemaphoreHandle_t  bufferMtx;
uint8_t buffer[2048];
uint8_t queueItem = 1;


// LED Definitions
#define RGB_BRIGHTNESS 10
#define LED_PIN 4

const int LEDsH = 8; // edit with your dimensions
const int LEDsW = 32;

#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel pixels(LEDsH* LEDsW, LED_PIN, NEO_GRB + NEO_KHZ800);

String displayString = "hello";
uint32_t displayColor = pixels.Color(10, 0, 10);

// Allow up to 1000 characters for the query
char query[1000];

#include "ledUtil.h"

void notFound(AsyncWebServerRequest* request) {
  request->send(404, "text/plain", "Not found");
}

void handlePost(AsyncWebServerRequest* request, uint8_t* data, size_t len, size_t index, size_t total) {

  Serial.println("received call to /api/v1/push");
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
    Serial.print(ts.getLabel(1)->key);
    Serial.print("=");
    Serial.println(ts.getLabel(1)->val);
    xSemaphoreGive(bufferMtx);
  }
}

String processor(const String& var) {
  if (var == "QUERY")
    return String(query);
  return String();
}

void setup()
{
  Serial.begin(115200);
  Serial.println(xPortGetCoreID());

  queue = xQueueCreate(1, sizeof(uint8_t));
  bufferMtx = xSemaphoreCreateMutex();

  if (queue == NULL) {
    Serial.println("Error creating the queue");
  }

  // pinMode(DATA_PIN, OUTPUT);

  neopixelWrite(RGB_BUILTIN, 1, 0, 0); // red


  // Configure and start the transport layer
  transport.setUseTls(true);
  transport.setCerts(grafanaCert, strlen(grafanaCert));
  transport.setWifiSsid(WIFI_SSID);
  transport.setWifiPass(WIFI_PASSWORD);
  transport.setNtpServer(NTP_SERVER);
  transport.setDebug(Serial);  // Remove this line to disable debug logging of the client.
  if (!transport.begin()) {
    Serial.println(transport.errmsg);
    while (true) {};
  }

  // Configure the client
  client.setUrl(GC_URL);
  client.setPath((char*)GC_PATH);
  //client.setQueryPath("/loki/api/v1");
  client.setQueryPath("/api/prom/api/v1");
  client.setPort(GC_PORT);
  client.setUser(GC_USER);
  client.setPass(GC_PASS);
  client.setDebug(Serial);  // Remove this line to disable debug logging of the client.
  if (!client.begin()) {
    Serial.println(client.errmsg);
    while (true) {};
  }

  writeRequest.addTimeSeries(ts);
  writeRequest.setDebug(Serial);

  readRequest.addTimeSeries(querySeries);
  readRequest.setDebug(Serial);


  // Load query from Preferences
  preferences.begin("query", false);
  size_t res = preferences.getString("query", query, sizeof(query));
  if (res > 0) {
    Serial.print("Loaded query from preferences: ");
    Serial.println(query);
  } else {
    Serial.println("Failed to load query from preferences");
    query[0] = '\0';
  }

  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send_P(200, "text/html", index_html, processor);
    });
  server.onNotFound(notFound);
  server.on("/api/v1/push", HTTP_POST, [](AsyncWebServerRequest* request) {
    request->send(200, "text/plain", "");
    }, NULL, handlePost);
  server.on("/set-query", HTTP_POST, [](AsyncWebServerRequest* request) {
    if (request->hasParam("query", true)) {
      AsyncWebParameter* p = request->getParam("query", true);
      if (p->value().length() > sizeof(query)) {
        Serial.println("Query too long");
        request->send(400, "text/plain", "Query too long");
        return;
      }
      strcpy(query, p->value().c_str());
      preferences.putString("query", query);
      Serial.print("Set query to: ");
      Serial.println(query);
      request->redirect("/");
    } else {
      request->send(400, "text/plain", "Missing query parameter");
    }
    });
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
  if (query[0] == '\0') {
    Serial.println("No query set, skipping");
    vTaskDelay(5000);
    return;
  }
  client.instantQuery(readRequest, (char*)query, strlen(query));
  Serial.println(querySeries.getSample(0)->val);
  pixels.clear();
  displayString = String(querySeries.getSample(0)->val);
  displayTextOnPanel(displayString, displayColor);
  vTaskDelay(5000);
}