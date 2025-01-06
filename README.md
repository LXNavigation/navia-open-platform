# navia-open-platform

## Arduino sketch

This Arduino IDE sketch implements a client for interacting with the Navia Core platform. Upon connecting to a specified network (e.g., a Navia Core Access Point), the sketch initially performs HTTP GET and POST requests to the Navia Core REST API. It then subscribes to the Navia Core WebSocket API to obtain and display the current QNH (altimeter setting) value.

Installation instructions:

1. Download and install Arduino IDE: https://www.arduino.cc/en/software

  Some features of ESP32 are not yet supported in Arduino IDE 2. In that case you can install legacy version 1.8.x.

2. Follow instructions on installing ESP32 Board in Arduino IDE 1.8.x: https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/

Or if you selected Arduino IDE 2.0: https://randomnerdtutorials.com/installing-esp32-arduino-ide-2-0/

Compile & Run instructions:

TBD

## Hardware

Hardware directory contains schematics and other reference documentation of LX ESP32 POE Development Board.
