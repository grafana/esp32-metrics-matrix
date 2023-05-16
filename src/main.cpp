#include <Arduino.h>
#include <WiFi.h>
#include <Preferences.h>
#include <esp_task_wdt.h>

#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>

#include "certificates.h"
#include "secrets.h"

#include <PromLokiTransport.h>
#include <PrometheusArduino.h>
#include "webserver.h"

PromLokiTransport transport;
PromClient client(transport);

//60 seconds WDT
#define WDT_TIMEOUT 60

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
uint8_t red, green, blue = 0;
uint8_t precision = 1;
uint32_t displayColor = dataArray.Color(10, 0, 10);

// Allow up to 1000 characters for the query
char query[1000];
String escapedQuery;
// Allow up to 11 characters for the label (10 + null terminator)
char label[11];
// Allow up to 6 characters for the label (5 + null terminator)
char unit[6];
// Allow up to 2 characters for the precision (1 + null terminator)
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

void htmlEncode(char* str, uint16_t len) {
  escapedQuery = "";
  for (uint16_t i = 0; i < len; i++) {
    switch (str[i]) {
      // The template parser uses %, so we encode these by doubling them
    case '%':
      escapedQuery += "%%";
      break;
    case '&':
      escapedQuery += "&amp;";
      break;
    case '<':
      escapedQuery += "&lt;";
      break;
    case '>':
      escapedQuery += "&gt;";
      break;
    case '"':
      escapedQuery += "&quot;";
      break;
    case '\'':
      escapedQuery += "&apos;";
      break;
    default:
      escapedQuery += str[i];
    }
  }
}

void htmlDecode(const String& encoded, char* output) {
  escapedQuery = encoded;
  escapedQuery.replace("%%", "%");
  escapedQuery.replace("&amp;", "&");
  escapedQuery.replace("&lt;", "<");
  escapedQuery.replace("&gt;", ">");
  escapedQuery.replace("&quot;", "\"");
  escapedQuery.replace("&apos;", "\'");
  escapedQuery.toCharArray(output, escapedQuery.length() + 1);
}

String processor(const String& var) {
  if (var == "QUERY") {
    htmlEncode(query, strlen(query));
    return escapedQuery;
  }
  else if (var == "LABEL") {
    htmlEncode(label, strlen(label));
    return escapedQuery;
  }
  else if (var == "UNIT") {
    htmlEncode(unit, strlen(unit));
    return escapedQuery;
  }
  else if (var == "PRECISION") {
    return String(precision);
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

void connectTask(void* pvParameters){ 
  char str[10];
  strcpy(str, ".");
  for (;;) {
    displayTextOnPanel(str, strlen(str), dataArray.Color(10, 0, 0), dataArray);
    strcat(str, ".");
    if (strlen(str) > 5) {
      strcpy(str, ".");
      dataArray.clear();
    }
    vTaskDelay(1000);
  }
}

void setup(){
  Serial.begin(115200);
  Serial.println(xPortGetCoreID());

  queue = xQueueCreate(1, sizeof(uint8_t));
  bufferMtx = xSemaphoreCreateMutex();

  if (queue == NULL) {
    Serial.println("Error creating the queue");
  }

  Serial.println("Configuring WDT...");
  esp_task_wdt_init(WDT_TIMEOUT, true); //enable panic so ESP32 restarts
  esp_task_wdt_add(NULL); //add current thread to WDT watch

  // pinMode(DATA_PIN, OUTPUT);

  neopixelWrite(RGB_BUILTIN, 1, 0, 0); // red


  // Configure and start the transport layer
  transport.setUseTls(true);
  transport.setCerts(grafanaCert, strlen(grafanaCert));
  transport.setWifiSsid(WIFI_SSID);
  transport.setWifiPass(WIFI_PASSWORD);
  transport.setNtpServer(NTP_SERVER);
  transport.setDebug(Serial);  // Remove this line to disable debug logging of the client.

  dataArray.clear();
  TaskHandle_t connectTaskHandle;
  xTaskCreate(connectTask, "ConnectTask", 1000, NULL, 2, &connectTaskHandle);

  if (!transport.begin()) {
    Serial.println(transport.errmsg);
    while (true) {};
  }

  vTaskDelete(connectTaskHandle);

  if (!MDNS.begin("esp32matrix")) {
    Serial.println("Error setting up MDNS responder!");
    while(1) {
        delay(1000);
    }
  }
  Serial.println("mDNS responder started");
  MDNS.addService("http", "tcp", 80);

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

  // Try to allocate some consistent space for the query, assuming the escaped version won't be more than double in size.
  escapedQuery.reserve(sizeof(query) * 2);

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
  res = preferences.getString("unit", unit, sizeof(unit));
  if (res > 0) {
    Serial.print("Loaded unit from preferences: ");
    Serial.println(unit);
  }
  else {
    Serial.println("Failed to load unit from preferences");
    unit[0] = '\0';
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
  precision = preferences.getUChar("precision", 1);
  Serial.print("Loaded precision value from preferences: ");
  Serial.println(precision);

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
        htmlDecode(p->value(), query);
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
        Serial.println("Label was empty, clearing");
        label[0] = '\0';
      }
      else {
        htmlDecode(p->value(), label);
      }
      preferences.putString("label", label);
      Serial.print("Set label to: ");
      Serial.println(label);
      labelArray.clear();
      displayTextOnPanel((const char*)label, strlen(label), labelArray.Color(red, green, blue), labelArray);
    }
    if (request->hasParam("unit", true)) {
      AsyncWebParameter* p = request->getParam("unit", true);
      if (p->value().length() > sizeof(unit)) {
        Serial.println("Unit too long");
        request->send(400, "text/plain", "Unit too long");
        return;
      }
      if (p->value().length() == 0) {
        Serial.println("Unit was empty, clearing unit");
        unit[0] = '\0';
      }
      else {
        htmlDecode(p->value(), unit);
      }
      preferences.putString("unit", unit);
      Serial.print("Set unit to: ");
      Serial.println(unit);
    }
    if (request->hasParam("precision", true)) {
      AsyncWebParameter* p = request->getParam("precision", true);
      precision = p->value().toInt();
      preferences.putUChar("precision", precision);
      Serial.print("Set precision to: ");
      Serial.println(precision);
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

uint8_t errorCnt = 0;

void loop()
{
  esp_task_wdt_reset();
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
    errorCnt++;
    if (errorCnt > 5) {
      ESP.restart();
    }
    return;
  }
  else {
    error[0] = '\0';
    errorCnt = 0;
  }
  dataArray.clear();
  double newVal = querySeries.getSample(0)->val;
  // This needs to be big enough to hold the value + unit + null terminator
  // Value is based on the number + the precision, hard to pin down so
  // picking a reasonably large number
  char newValStr[30];

  dtostrf(newVal, 0, precision, newValStr);
  Serial.println(newValStr);

  if (strlen(unit) > 0) {
    strcat(newValStr, unit);
  }

  displayTextOnPanel(newValStr, strlen(newValStr), dataArray.Color(red, green, blue), dataArray);

  vTaskDelay(5000);
}