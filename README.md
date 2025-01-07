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

4. Open and Upload sketch: https://github.com/LXNavigation/navia-open-platform/tree/main/NaviaOpenExample

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

### Board Overview

The development board is built around the ESP-WROOM-32 module, leveraging its powerful dual-core processor, Wi-Fi, and Bluetooth capabilities. The board exposes a variety of GPIO pins, power options, and other features to facilitate easy prototyping and development.

![Navia ESP32 POE Development Board](https://github.com/LXNavigation/navia-open-platform/blob/main/Hardware/pcb.png?raw=true)

### Pinout Diagram

![Navia ESP32 POE Development Board](https://github.com/LXNavigation/navia-open-platform/blob/main/Hardware/ESP32-POE-DEV%20Preview.png?raw=true)

A preview of our development board is shown above, illustrating the physical layout of the pins.

### Pin Descriptions

The following tables detail the available pins on the development board and their corresponding GPIO numbers on the ESP32.

#### Left Side Pins

| Pin Label | ESP32 GPIO | Notes                                      |
| --------- | ----------- | ----------------------------------------- |
| 5V        | -           | 5V Power Output                           |
| GND       | -           | Ground                                    |
| VCC       | -           | 3.3V Power Output                         |
| IO39      | GPIO39      | Input Only                                |
| IO36      | GPIO36      | Input Only                                |
| IO35      | GPIO35      | Input Only                                |
| IO34      | GPIO34      | Input Only                                |
| IO33      | GPIO33      | ADC1_CH5, Touch Sensing, PWM capable      |
| IO32      | GPIO32      | ADC1_CH4, Touch Sensing, PWM capable      |
| IO17      | GPIO17      | Can be used as general I/O    |

#### Right Side Pins

| Pin Label | ESP32 GPIO | Notes                                      |
| --------- | ----------- | ----------------------------------------- |
| 5V        | -           | 5V Power Output                           |
| GND       | -           | Ground                                    |
| VCC       | -           | 3.3V Power from ESP32 module              |
| IO15      | GPIO15      | ADC2_CH3, HSPI_CS, PWM capable, MTDO      |
| IO14      | GPIO14      | ADC2_CH6, HSPI_CLK, PWM capable, MTCK     |
| IO13      | GPIO13      | ADC2_CH4, HSPI_MOSI, PWM capable, MTDI, DAC2_OUT |
| IO12      | GPIO12      | ADC2_CH5, HSPI_MISO, PWM capable, MTMS    |
| IO5       | GPIO5       | VSPI_CS0, PWM capable                     |
| IO4       | GPIO4       | ADC2_CH0, Touch Sensing, PWM capable      |
| IO2       | GPIO2       | ADC2_CH2, Touch Sensing, PWM capable      |
| GND       | -           | Ground                                    |
| 48V       | -           | 48V Power Output                          |

#### Notes on Pin Usage

*   **ADC:** Pins labeled with ADC can be used for analog-to-digital conversion. Note that ADC2 pins cannot be used when Wi-Fi is active.
*   **Touch Sensing:** Certain pins support capacitive touch sensing.
*   **PWM:** Pins labeled as PWM capable can be used for pulse-width modulation to control devices like motors or LEDs.
*   **SPI:** The board supports SPI communication using specific pins (HSPI and VSPI).
*   **Input Only:** GPIOs 34, 35, 36, and 39 are input-only pins.
*   **Power Pins:** The board provides 5V, 3.3V, and a dedicated 48V for specialized applications. Be careful not to exceed the voltage tolerances of the ESP32 (3.3V) on its GPIO pins.
*   **Boot Mode Selection:** Some pins (like GPIO0, not exposed on our board's external pins) might have special functions during boot. Refer to the ESP32 datasheet for more details.

### Power Supply

The board can be powered via:

*   **5V Pin:** Supply 5V through the designated 5V pins.
*   **USB:**  The board include a micro USB connector. Use to upload your sketch and monitor serial output. 
*   **48V Pin:** For high-voltage applications, use 48V pin with caution.
  
**Important:** Always refer to the ESP32 datasheet and the schematic of this development board for the most accurate and up-to-date information before connecting external components or using specific pins.

### Further Information

*   **LX POE Development board Schematics:** \[https://github.com/LXNavigation/navia-open-platform/blob/main/Hardware/ESP32-POE-DEV%20Schematic.pdf]
*   **ESP-IDF Pinout:** \[https://github.com/LXNavigation/navia-open-platform/blob/main/Hardware/ESP32-POE-DEV%20Preview.pdf]

We hope this guide is helpful for your projects. If you have any questions or suggestions, please feel free to open an issue or contribute to this repository!

