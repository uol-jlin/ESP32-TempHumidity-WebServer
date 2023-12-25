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
#define DHTPIN 33      // DHT sensor pin
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
<!DOCTYPE HTML>
<html>
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
    .slider-container {
      margin: 15px 0;
    }
    .alert {
      color: red;
      font-weight: bold;
      margin-top: 20px;
      display: none;
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
        <span id="temperature" class="d-inline">%TEMPERATURE%&deg;C</span>
      </div>
    </div>
    <div class="card text-center">
      <div class="card-header">
        Humidity
      </div>
      <div class="card-body">
        <i class="fas fa-tint card-icon"></i>
        <span id="humidity" class="d-inline">%HUMIDITY%&percnt;</span>
      </div>
    </div>

    <div class="card text-center">
      <div class="card-header">Set Optimal Values</div>
      <div class="card-body">
        <div class="slider-container">
          <label for="tempSlider">Temperature:</label>
          <input type="range" id="tempSlider" class="slider" min="0" max="50">
          <span id="tempSetting">25&deg;C</span>
        </div>
        <div class="slider-container">
          <label for="humidSlider">Humidity:</label>
          <input type="range" id="humidSlider" class="slider" min="0" max="100">
          <span id="humidSetting">35&percnt;</span>
        </div>
        <button id="saveSettings" class="btn btn-primary">Save Preferences</button>
      </div>
      <div id="saveConfirmation" class="alert alert-success" style="display: none; background-color: #28a745; color:
        #fff; padding: 10px 40px; margin: 20px 20px;">  
        Preferences saved successfully!
      </div>
      <div id="alertDiv" class="alert" style="color: black; font-weight: bold; margin-top: 20px; display: block;">
        Monitoring temperature and humidity...
      </div>
    </div>
  </div>

  <script>
    var optimalTemp = 25;
    var optimalHumidity = 50;

    const tempSettingElement = document.getElementById("tempSetting");
    const humidSettingElement = document.getElementById("humidSetting");
    const saveConfirmation = document.getElementById("saveConfirmation");

    document.getElementById("tempSlider").oninput = function() {
      tempSettingElement.innerHTML = this.value + '&deg;C'; 
      optimalTemp = this.value;
    }

    document.getElementById("humidSlider").oninput = function() {
      humidSettingElement.innerHTML = this.value + '&percnt;'; 
      optimalHumidity = this.value;
    }
    document.getElementById("saveSettings").onclick = function() {
      saveConfirmation.style.display = 'block';
      setTimeout(function() {
        saveConfirmation.style.display = 'none';
      }, 3000
      ); 
    }

    function updateAlertMessage(currentTemp, currentHumidity) {
      if (Math.abs(currentTemp - optimalTemp) > 3 || Math.abs(currentHumidity - optimalHumidity) > 3) {
        alertDiv.style.display = "block"; 
        alertDiv.style.color = "red";
        alertDiv.innerHTML = "Conditions deviating from set range!";
      } else {
        alertDiv.style.display = "block"; 
        alertDiv.style.color = "green";
        alertDiv.innerHTML = "We are in optimal conditions.";
      }
    }

    setInterval(function() {
      var xhttpTemp = new XMLHttpRequest();
      xhttpTemp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          const currentTemp = parseFloat(this.responseText);
          document.getElementById("temperature").innerHTML = currentTemp + '&deg;C';
          updateAlertMessage(optimalTemp, currentHumidity);
        }
      };
      xhttpTemp.open("GET", "/temperature", true);
      xhttpTemp.send();

      var xhttpHumidity = new XMLHttpRequest();
      xhttpHumidity.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          const currentHumidity = parseFloat(this.responseText);
          document.getElementById("humidity").innerHTML = currentHumidity + '&percnt;';
          updateAlertMessage(optimalTemp, currentHumidity);
        }
      };
      xhttpHumidity.open("GET", "/humidity", true);
      xhttpHumidity.send();
    }, 10000);
  </script>
</body>
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

  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send_P(200, "text/plain", readDHTTemperature().c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send_P(200, "text/plain", readDHTHumidity().c_str());
  });

  server.begin();
}

void loop() {
}
