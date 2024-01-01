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

#define RED_LED_PIN 5
#define GREEN_LED_PIN 18
#define BLUE_LED_PIN 19

#define BUZZER_PIN 21

float optimalTemp = 25;
float optimalHumidity = 35;

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
    <link href="https://fonts.googleapis.com/css2?family=Montserrat:wght@300;400;700&display=swap" rel="stylesheet">
    <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css">
    <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
    <style>
        body {
            font-family: 'Montserrat', sans-serif;
            background-color: #f4f7f6;
            padding-top: 20px;
            margin: 0;
            color: #333;
        }
        .navbar-brand {
            font-weight: 700;
            font-size: 1.5rem;
            padding-bottom: 5px;
        }
        .container {
            max-width: 1000px;
            margin: 20px auto; 
            background-color: #fff;
            border-radius: 12px;
            box-shadow: 0 6px 15px rgba(0, 0, 0, 0.1);
            padding: 25px;
        }
        .card {
            border: 1px solid #007bff;
            border-radius: 10px;
            transition: all 0.3s ease-in-out;
            overflow: hidden;
        }
        .card:hover {
            transform: translateY(-5px);
            box-shadow: 0 6px 20px rgba(0, 0, 0, 0.15);
        }
        .card-header {
            background-color: #007bff;
            color: white;
            font-size: 1.25rem;
            padding: 15px 20px;
        }
        .card-body {
            position: relative;
            padding: 20px;
        }
        .card-icon {
            color: #007bff;
            font-size: 3rem;
            margin-bottom: 10px;
        }
        .sensor-value {
            font-size: 2rem;
            display: block;
            margin-top: 10px;
        }
        .custom-alert-margin {
            margin-top: 1rem;
            margin-bottom: 1rem; 
        }
        .slider-container {
            background-color: #f9fafb;
            padding: 20px;
            border-radius: 12px;
            box-shadow: 0 4px 8px rgba(0, 0, 0, 0.05);
            margin-bottom: 15px;
        }
        .form-control-range {
            height: 2.5rem;
            padding: 5px 0;
            background-color: #eef1f3;
            border-radius: 10px;
        }
        .chart-container {
            position: relative;
            height: 400px;
            margin-top: 10px;
            margin-bottom: 80px;
        }
        .chart-title {
            text-align: center;
            font-size: 1.5rem;
            margin-bottom: 20px;
            margin-top: 40px;
            font-weight: 600;
        }
        .btn-primary {
            background-color: #007bff;
            border: none;
            border-radius: 8px;
            padding: 10px 15px;
            font-weight: 600;
        }
        .alert {
            border-radius: 8px;
            display: none;
        }
    </style>
</head>
<body>
    <nav class="navbar navbar-expand-lg navbar-light">
        <div class="container">
            <a class="navbar-brand" href="#">ESP32 Climate Dashboard</a>
        </div>
    </nav>
    <div class="container mt-1">
        <div class="row">
            <div class="col-lg-6 mb-4">
                <div class="card text-center">
                    <div class="card-header">Temperature</div>
                    <div class="card-body">
                        <i class="fas fa-thermometer-half card-icon"></i>
                        <span id="temperature" class="sensor-value d-inline-block">Loading...</span>
                    </div>
                </div>
            </div>
            <div class="col-lg-6 mb-4">
                <div class="card text-center">
                    <div class="card-header">Humidity</div>
                    <div class="card-body">
                        <i class="fas fa-tint card-icon"></i>
                        <span id="humidity" class="sensor-value d-inline-block">Loading...</span>
                    </div>
                </div>
            </div>
        </div>
        <div class="slider-container">
            <h3 class="text-center">Set Optimal Values</h3>
            <div class="form-group">
                <label for="tempSlider">Temperature:</label>
                <input type="range" id="tempSlider" class="form-control-range" min="0" max="50">
                <span id="tempSetting" class="float-right">25&deg;C</span>
            </div>
            <div class="form-group">
                <label for="humidSlider">Humidity:</label>
                <input type="range" id="humidSlider" class="form-control-range" min="0" max="100">
                <span id="humidSetting" class="float-right">35&percnt;</span>
            </div>
        </div>
        <button id="saveSettings" class="btn btn-primary btn-block custom-alert-margin">Save Preferences</button>
        <div id="saveConfirmation" class="alert alert-success text-center">Preferences saved successfully.</div>
        <div id="alertDiv" class="alert alert-danger text-center"></div>
        <div class="chart-container">
            <h3 class="chart-title">Temperature Chart</h3>
            <canvas id="temperatureChart"></canvas>
        </div>
        <div class="chart-container">
            <h3 class="chart-title">Humidity Chart</h3>
            <canvas id="humidityChart"></canvas>
        </div>
    </div>

  <script>
    var currentTemp = 0;
    var currentHumidity = 0;

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

    function updateAlertMessage() {
      var alertDiv = document.getElementById('alertDiv');
      var tempDeviation = Math.abs(currentTemp - optimalTemp);
      var humidDeviation = Math.abs(currentHumidity - optimalHumidity);
      var maxDeviation = Math.max(tempDeviation, humidDeviation);

      var tempMessage = currentTemp > optimalTemp ? `\nTemperature too high by ${tempDeviation.toFixed(1)}&deg;C` : `\nTemperature too low by ${tempDeviation.toFixed(1)}&deg;C`;
      var humidMessage = currentHumidity > optimalHumidity ? `\nHumidity too high by ${humidDeviation.toFixed(1)}&percnt;` : `\nHumidity too low by ${humidDeviation.toFixed(1)}&percnt;`;

      if (maxDeviation > 4) {
          alertDiv.style.display = "block";
          alertDiv.className = "alert alert-danger text-center custom-alert-margin";
          alertDiv.innerHTML = `High deviation detected! ${tempMessage}; ${humidMessage}`;
          setLEDBuzzer(255, 0, 0, true); 
      } else if (maxDeviation > 2) {
          alertDiv.style.display = "block";
          alertDiv.className = "alert alert-warning text-center custom-alert-margin";
          alertDiv.innerHTML = `Moderate deviation detected! ${tempMessage}; ${humidMessage}`;
          setLEDBuzzer(255, 255, 0, false);
      } else {
          alertDiv.style.display = "block";
          alertDiv.className = "alert alert-success text-center custom-alert-margin";
          alertDiv.innerHTML = "Conditions are optimal.";
          setLEDBuzzer(0, 255, 0, false); 
      }
    }

    function setLEDBuzzer(r, g, b, buzzer) {
        console.log("Setting LED and Buzzer:", r, g, b, buzzer);
        var xhr = new XMLHttpRequest(); 
        xhr.open("GET", "/updateAlert?r=" + r + "&g=" + g + "&b=" + b + "&buzzer=" + (buzzer ? 1 : 0), true);
        xhr.send();
    }

    var temperatureData = [];
    var humidityData = [];
    var labels = [];

    var tempCtx = document.getElementById('temperatureChart').getContext('2d');
    var temperatureChart = new Chart(tempCtx, {
      type: 'line',
      data: {
        labels: labels,
        datasets: [
          {
            label: 'Temperature (\u00B0C)',
            data: temperatureData,
            borderColor: 'rgb(255, 99, 132)',
            fill: true
          },
          {
            label: 'Optimal Temperature (\u00B0C)',
            data: [], 
            borderColor: 'green', 
            fill: true
          }
        ]
      },
      options: {}
    });

    var humidCtx = document.getElementById('humidityChart').getContext('2d');
    var humidityChart = new Chart(humidCtx, {
      type: 'line',
      data: {
        labels: labels,
        datasets: [
          {
            label: 'Humidity (\u0025)',
            data: humidityData,
            borderColor: 'rgb(54, 162, 235)',
            fill: true
          },
          {
            label: 'Optimal Humidity (\u0025)',
            data: [], 
            borderColor: 'green', 
            fill: true
          }
        ]
      },
      options: {}
    });

    const addData = (chart, label, data, optimalData) => {
      chart.data.labels.push(label);
      chart.data.datasets[0].data.push(data); 
      chart.data.datasets[1].data.push(optimalData);
      chart.update();
    };

    const getCurrentTime = () => {
      const now = new Date();
      let hours = now.getHours();
      let minutes = now.getMinutes().toString().padStart(2, '0');
      let seconds = now.getSeconds().toString().padStart(2, '0');
      return `${hours}:${minutes}:${seconds}`;
    };

    setInterval(function() {
      var xhttpTemp = new XMLHttpRequest();
      xhttpTemp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          currentTemp = parseFloat(this.responseText);
          const currentTime = getCurrentTime(); 
          document.getElementById("temperature").innerHTML = currentTemp + '&deg;C';
          addData(temperatureChart, currentTime, currentTemp, optimalTemp);
        }
      };
      xhttpTemp.open("GET", "/temperature", true);
      xhttpTemp.send();

      var xhttpHumidity = new XMLHttpRequest();
      xhttpHumidity.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          currentHumidity = parseFloat(this.responseText);
          const currentTime = getCurrentTime(); 
          document.getElementById("humidity").innerHTML = currentHumidity + '&percnt;';
          addData(humidityChart, currentTime, currentHumidity, optimalHumidity);
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

void updateAlertStatus(bool red, bool green, bool blue, bool buzzer) {
    digitalWrite(RED_LED_PIN, red ? HIGH : LOW);
    digitalWrite(GREEN_LED_PIN, green ? HIGH : LOW);
    digitalWrite(BLUE_LED_PIN, blue ? HIGH : LOW);
    digitalWrite(BUZZER_PIN, buzzer ? HIGH : LOW); 
}

void setup() {
  Serial.begin(115200);
  dht.begin();

  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);

  pinMode(BUZZER_PIN, OUTPUT);

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
  server.on("/updateAlert", HTTP_GET, [](AsyncWebServerRequest *request) {
      int red = request->getParam("r")->value().toInt();
      int green = request->getParam("g")->value().toInt();
      int blue = request->getParam("b")->value().toInt();
      bool buzzer = request->getParam("buzzer")->value().toInt() == 1;
      updateAlertStatus(red, green, blue, buzzer);
      request->send(200, "text/plain", "OK");
  });
  server.begin();
};

void loop() {
}
