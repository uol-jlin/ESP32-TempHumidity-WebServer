// Import required libraries
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include "config.h"

// Network credentials
extern const char* ssid;
extern const char* password;

// Sensor configuration
#define DHTPIN 33    // DHT sensor pin
#define DHTTYPE DHT22  // DHT 22 (AM2302)

DHT dht(DHTPIN, DHTTYPE);
AsyncWebServer server(80);

String readDHTTemperature() {
  float t = dht.readTemperature();
  if (isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  return String(t);
}

String readDHTHumidity() {
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  return String(h);
}

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css">
  <style>
    body {
      font-family: 'Arial', sans-serif;
      background-color: #e9ecef;
      padding-top: 20px;
    }
    .container {
      max-width: 800px;
      margin: auto;
      padding-bottom: 20px;
    }
    .card {
      box-shadow: 0 2px 10px rgba(0,0,0,0.1);
      margin-top: 20px;
    }
    .card-icon {
      font-size: 48px;
      color: #017bff;
    }
    .card-header {
      font-size: 1.5rem;
      color: #fff;
      background-color: #017bff;
    }
    .card-body {
      font-size: 2.5rem;
      color: #333;
    }
    .units {
      font-size: 1.2rem;
      color: #666;
    }
  </style>
</head>
<body>
  <div class="container">
    <h2 class="text-center">ESP32 Climate Dashboard</h2>
    <div class="card text-center">
      <div class="card-header">
        Temperature
      </div>
      <div class="card-body">
        <i class="fas fa-thermometer-half card-icon"></i>
        <p id="temperature" class="d-inline">%TEMPERATURE%<sup class="units">&deg;C</sup></p>
      </div>
    </div>
    <div class="card text-center">
      <div class="card-header">
        Humidity
      </div>
      <div class="card-body">
        <i class="fas fa-tint card-icon"></i>
        <p id="humidity" class="d-inline">%HUMIDITY%<sup class="units">&percnt;</sup></p>
      </div>
    </div>
  </div>
</body>
<script>

setInterval(function () {
  var xhttpTemp = new XMLHttpRequest();
  xhttpTemp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      const currentTemp = parseFloat(this.responseText);
      document.getElementById("temperature").innerHTML = currentTemp;
    }
  };
  xhttpTemp.open("GET", "/temperature", true);
  xhttpTemp.send();

  var xhttpHumidity = new XMLHttpRequest();
  xhttpHumidity.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      const currentHumidity = parseFloat(this.responseText);
      document.getElementById("humidity").innerHTML = currentHumidity;
    }
  };
  xhttpHumidity.open("GET", "/humidity", true);
  xhttpHumidity.send();
}, 10000);
</script>
</html>)rawliteral";

String processor(const String& var) {
  if (var == "TEMPERATURE") return readDHTTemperature();
  if (var == "HUMIDITY") return readDHTHumidity();
  return String();
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/plain", readDHTTemperature().c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/plain", readDHTHumidity().c_str());
  });

  server.begin();
}

void loop() {
}
