# navia-open-platform

## Arduino sketch

This Arduino IDE sketch implements a client for interacting with the Navia Core platform. Upon connecting to a specified network (e.g., a Navia Core Access Point), the sketch initially performs HTTP GET and POST requests to the Navia Core REST API. It then subscribes to the Navia Core WebSocket API to obtain and display the current QNH (altimeter setting) value.

**Installation instructions:**

1. Download and install Arduino IDE: https://www.arduino.cc/en/software

  Some features of ESP32 are not yet supported in Arduino IDE 2. In that case you can install legacy version 1.8.x.

2. Follow instructions on installing ESP32 Board in Arduino IDE 1.8.x: https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/

Or if you selected Arduino IDE 2.0: https://randomnerdtutorials.com/installing-esp32-arduino-ide-2-0/

**Compile & Run instructions:**

In Arduino IDE 1.8.x:

1. Select:
   
   Tools > Board: "ESP32 Dev Module"
   
   Tools > Flash Size: "4MB (32Mb)"
   
   Tools > Port: \<select-your-com-port\>

4. Upload sketch

5. Open: Tools > Serial Monitor.

**Expected output:**
<code>
Navia Open Example has taken off!
Connecting to network: LXN
......
Connected to WiFi network with IP Address: 192.168.1.164
Setting up WebSocket client
[REST] Requesting: GET http://192.168.1.184:3811/v1/info
[REST] HTTP Response code: 200
{
    "hardware_id": 0,
    "previous_update": "none",
    "seal_valid": false,
    "sensors": {
        "airspeed_sensor": 7,
        "altimeter_sensor": 1,
        "temp_sensor": 4,
        "vario_sensor": 2
    },
    "serial": 0,
    "serial_date": 2460608,
    "serial_igc": "000",
    "vendor": 0,
    "version_be": "0.0.test",
    "version_fe": "0.0.dev",
    "version_navia": "0.0.7d4f5cd.test"
}
[REST] Requesting: POST http://192.168.1.184:3811/v1/user/login
14:55:08.438 [RX] - [REST] HTTP Response code: 200
{"message":"Same user loggedin again on device","response data":{"currentUser":{"email":"test@test.si","firstName":"John","lastName":"McFlyer","roleId":4},"devaToken":"b86b3cfb-a1e5-4bfc-af51-6184bd868aae"}}
[REST] Requesting: POST http://192.168.1.184:3811/v1/command
[REST] HTTP Response code: 200
[WSCK] Connected to url: /ws<LF>
[WSCK] Received text: {"update":"baro correction","value":1013.2}<LF>
[WSCK] Received text: {"update":"baro correction","value":1013.2}<LF>
[WSCK] Received text: {"update":"baro correction","value":1013.2}<LF>
</code>

## Hardware

Hardware directory contains schematics and other reference documentation of LX ESP32 POE Development Board.

![Navia ESP32 POE Development Board](https://github.com/LXNavigation/navia-open-platform/blob/main/Hardware/pcb.png?raw=true)
