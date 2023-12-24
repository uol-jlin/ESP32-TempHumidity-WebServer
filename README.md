# ESP32 Temperature Humidity Web Server

## Project Overview
The ESP32 Climate Monitor is an IoT project that utilizes the ESP32 microcontroller for real-time temperature and humidity monitoring. Leveraging a DHT22 sensor, this project offers a web server interface to display the sensor readings, making it accessible remotely.

## Features
- Real-time temperature and humidity monitoring using the DHT22 sensor.
- Web server interface for easy access to sensor data.
- Continuous data acquisition and server responsiveness for reliable monitoring.

## Hardware Requirements
- ESP32 Development Board (Micro USB port)
- DHT22 Temperature and Humidity Sensor
- Jumper Wires
- Micro USB to USB/USB-C cable for programming and power supply (depending on the available port on your computer)

## Software and Libraries
This project uses the following libraries:
- [Arduino IDE](https://www.arduino.cc/en/Main/Software)
- [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)
- [Adafruit_Sensor](https://github.com/adafruit/Adafruit_Sensor)
- [DHT sensor library](https://github.com/adafruit/DHT-sensor-library)

## Setup and Installation
- Assemble the hardware following the ESP32 to DHT22 connection standards.
- Install the required libraries in the Arduino IDE.
- Upload the provided sketch to the ESP32.
- Connect the ESP32 to your WiFi network by updating the SSID and password in `config.h`.

## Web Server Access
Access the web interface by navigating to the ESP32's IP address, displayed on the serial monitor after connecting to WiFi. The interface displays real-time temperature and humidity readings.

## Future Enhancements
- Integration with cloud services for advanced data logging.
- Implementing OTA (Over The Air) updates for the ESP32.
- Expansion with additional environmental sensors for comprehensive monitoring.

## Contributing
Contributions to this project are welcome! Feel free to fork the repository and submit pull requests, or send your suggestions and feedback.

## License
This project is open source and available under the MIT License.

## Contact
For any queries or suggestions, please reach out at uoljlin@gmail.com.
