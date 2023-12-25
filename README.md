# ESP32 Temperature and Humidity Web Server

## Project Overview
The ESP32 Climate Monitor is an interactive IoT project that employs the ESP32 microcontroller to monitor temperature and humidity in real-time. Using a DHT22 sensor, it provides a web server interface for remote access to live sensor data.

## Features
- Real-time monitoring of temperature and humidity with the DHT22 sensor.
- Web server interface for remote data visualization.
- User-specified optimal climate settings with visual feedback through an RGB LED.
- Audible alerts via a piezo buzzer when conditions exceed the user-defined optimal range.

## Hardware Requirements
- ESP32 Development Board
- DHT22 Temperature and Humidity Sensor
- RGB LED
- Piezo Buzzer
- Jumper Wires
- Micro USB to USB cable for programming and power
- Resistors for LED and buzzer protection

## Software and Libraries
Required libraries for this project:
- [Arduino IDE](https://www.arduino.cc/en/Main/Software)
- [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)
- [Adafruit_Sensor](https://github.com/adafruit/Adafruit_Sensor)
- [DHT sensor library](https://github.com/adafruit/DHT-sensor-library)

## Setup and Installation
1. Assemble the hardware by connecting the DHT22, RGB LED, and piezo buzzer to the ESP32.
2. Install the Arduino IDE and necessary libraries.
3. Upload the provided code to the ESP32.
4. Set up the WiFi credentials in `config.h` to connect the ESP32 to the internet.

## Web Server Access
The web interface can be accessed through the ESP32's IP address, displayed in the serial monitor after WiFi connection. It provides the current temperature and humidity readings and allows users to set their optimal climate conditions.

## Optimal Climate Settings
- Users can specify their optimal temperature and humidity settings via the web interface.
- The predefined optimal range is visually indicated by the RGB LED: green for optimal, yellow for warning, and red for critical conditions.
- The buzzer will sound if the readings fall outside the user-specified optimal range.

## Future Enhancements
- Cloud service integration for data logging and analysis.
- OTA updates for seamless firmware upgrades.
- Extension with more sensors for a comprehensive environmental monitoring system.

## Contributing
Your contributions are welcome! Please fork the repository, make your changes, and submit a pull request. For suggestions and feedback, reach out to us.

## License
This project is licensed under the MIT License - see the LICENSE file for details.

## Contact
For inquiries or suggestions, contact us at uoljlin@gmail.com.
