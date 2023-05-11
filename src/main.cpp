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
//#define RGB_BRIGHTNESS 10

#define DATA_LED_PIN 4
#define LABEL_LED_PIN 5

const int LEDsH = 8; // edit with your dimensions
const int LEDsW = 32;

#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel labelArray(LEDsH* LEDsW, LABEL_LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel dataArray(LEDsH* LEDsW, DATA_LED_PIN, NEO_GRB + NEO_KHZ800);

String displayString = "hello";
uint8_t red,green,blue = 0;
uint32_t displayColor = dataArray.Color(10, 0, 10);

// Allow up to 1000 characters for the query
char query[1000];
// Allow up to 11 characters for the label (10 + null terminator)
char label[11];
// Allow up to 1000 characters for the error message
char error[1000];

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
  if (var == "QUERY") {
    return String(query);
  }
  else if (var == "LABEL") {
    return String(label);
  }
  else if (var == "ERROR") {
    return String(error);
  }
  else if (var == "RED") {
    return String(red);
  }
  else if (var == "GREEN") {
    return String(green);
  }
  else if (var == "BLUE") {
    return String(blue);
  }

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
  }
  else {
    Serial.println("Failed to load query from preferences");
    query[0] = '\0';
  }
  res = preferences.getString("label", label, sizeof(label));
  if (res > 0) {
    Serial.print("Loaded label from preferences: ");
    Serial.println(label);
  }
  else {
    Serial.println("Failed to load label from preferences");
    label[0] = '\0';
  }
  red = preferences.getUChar("red", 10);
  Serial.print("Loaded red value from preferences: ");
  Serial.println(red);
  green = preferences.getUChar("green", 0);
  Serial.print("Loaded green value from preferences: ");
  Serial.println(green);
  blue = preferences.getUChar("blue", 10);
  Serial.print("Loaded blue value from preferences: ");
  Serial.println(blue);
  
  error[0] = '\0';

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
      if (p->value().length() == 0) {
        Serial.println("Query was empty, ignoring");
      }
      else {
        strcpy(query, p->value().c_str());
        preferences.putString("query", query);
        Serial.print("Set query to: ");
        Serial.println(query);
      }
    }
    else {
      request->send(400, "text/plain", "Missing query parameter");
    }

    if (request->hasParam("red", true)) {
      AsyncWebParameter* p = request->getParam("red", true);
      red = p->value().toInt();
      preferences.putUChar("red", red);
      Serial.print("Set red to: ");
      Serial.println(red);
    }
    if (request->hasParam("green", true)) {
      AsyncWebParameter* p = request->getParam("green", true);
      green = p->value().toInt();
      preferences.putUChar("green", green);
      Serial.print("Set green to: ");
      Serial.println(green);
    }
    if (request->hasParam("blue", true)) {
      AsyncWebParameter* p = request->getParam("blue", true);
      blue = p->value().toInt();
      preferences.putUChar("blue", blue);
      Serial.print("Set blue to: ");
      Serial.println(blue);
    }

    if (request->hasParam("label", true)) {
      AsyncWebParameter* p = request->getParam("label", true);
      if (p->value().length() > sizeof(label)) {
        Serial.println("Label too long");
        request->send(400, "text/plain", "Label too long");
        return;
      }
      if (p->value().length() == 0) {
        Serial.println("Label was empty, ignoring");
      }
      else {
        strcpy(label, p->value().c_str());
        preferences.putString("label", label);
        Serial.print("Set label to: ");
        Serial.println(label);
        labelArray.clear();
        displayTextOnPanel((const char*)label, strlen(label), labelArray.Color(red, green, blue), labelArray);
      }
    }
    request->redirect("/");
    });
  server.begin();
  Serial.print("Stack high water mark: ");
  Serial.println(uxTaskGetStackHighWaterMark(NULL));

  // TODO I don't know that this is needed anymore, we could maybe just put it back in loop()
  // Snappy decompression needs a bigger stack, so we create a new task where we can define a bigger stack.
  // xTaskCreate(processTask, "ProcessTask", 32768, NULL, 1, NULL);
  xTaskCreatePinnedToCore(processTask, "ProcessTask", 8192, NULL, 1, NULL, 1);

  // Clear the display

  labelArray.clear();
  displayTextOnPanel((const char*)label, strlen(label), labelArray.Color(red, green, blue), labelArray);


  dataArray.clear();
  dataArray.show();

}

void loop()
{
  if (query[0] == '\0') {
    Serial.println("No query set, skipping");
    vTaskDelay(5000);
    return;
  }
  PromClient::SendResult res = client.instantQuery(readRequest, (char*)query, strlen(query));
  if (res != PromClient::SendResult::SUCCESS) {
    Serial.print("Error sending query: ");
    Serial.println(res);
    Serial.println(client.errmsg);
    strcpy(error, client.errmsg);
    vTaskDelay(5000);
    return;
  }
  else {
    error[0] = '\0';
  }
  dataArray.clear();
  double newVal = querySeries.getSample(0)->val;
  char newValStr[20];
  dtostrf(newVal, 0, 2, newValStr);
  Serial.println(newValStr);
  displayTextOnPanel(newValStr, strlen(newValStr), dataArray.Color(red, green, blue), dataArray);
  vTaskDelay(5000);
}