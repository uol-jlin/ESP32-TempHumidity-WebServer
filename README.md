# ESP32 Temperature and Humidity Web Server

## Project Overview
The ESP32 Climate Monitor is a practical IoT application that utilizes an ESP32 microcontroller to measure and monitor temperature and humidity in real-time. The system reads data from a DHT22 sensor and provides a web server interface for remote viewing.

## Features
- Real-time temperature and humidity monitoring with the DHT22 sensor.
- Web server interface for remote access to live sensor data.
- Visual feedback on climate conditions through an RGB LED.
- Audible alarms with a piezo buzzer when deviations occur from the user-defined optimal range.

## Hardware Requirements
- ESP32 Development Board
- DHT22 Temperature and Humidity Sensor
- RGB LED
- Piezo Buzzer
- Breadboard for non-direct connections
- Jumper Wires
- Resistors for LED and buzzer
- Power supply via micro USB

## Software and Libraries
The project requires the following libraries:
- [Arduino IDE](https://www.arduino.cc/en/Main/Software)
- [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)
- [Adafruit_Sensor](https://github.com/adafruit/Adafruit_Sensor)
- [DHT sensor library](https://github.com/adafruit/DHT-sensor-library)

The front end uses [Chart.js](https://www.chartjs.org/) for rendering real-time graphs.

## Setup and Installation
1. Connect the DHT22, RGB LED, and piezo buzzer to the ESP32 using a breadboard for complex wiring.
2. Install the Arduino IDE and import the necessary libraries.
3. Load the sketch onto the ESP32 board.
4. Configure the network settings in the provided `config.h` file or directly in the sketch.

## Web Server Access
The web server can be accessed at the ESP32's IP address, which is printed to the serial monitor upon successful connection to the network.

## Optimal Climate Settings
The web interface allows users to set optimal temperature and humidity ranges. The LED color changes according to these settings: green for normal, yellow for caution, and red for high alert. The buzzer sounds when the sensor readings exceed the set thresholds.

## Real-time Graphs
The web interface includes graphs that display historical temperature and humidity data updated in real-time.

## Future Enhancements
Planned enhancements include cloud data logging, OTA firmware updates, and additional sensor support.

## Contributing
Contributions are welcome. Please fork the repository, commit your changes, and open a pull request.

## License
This project is open-sourced under the MIT License.

## Contact
For any queries or feedback, please email us at uoljlin@gmail.com.
