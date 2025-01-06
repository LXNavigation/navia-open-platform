/**
 * @file NaviaOpenSample.ino
 * @author LX Navigation
 * @brief This Arduino sketch implements a client for interacting with the Navia
 * Core platform. Upon connecting to a specified network (e.g., a Navia Core
 * Access Point), the sketch initially performs HTTP GET and POST requests to
 * the Navia Core REST API. It then subscribes to the Navia Core WebSocket API
 * to obtain and display the current QNH (altimeter setting) value.
 * @version 1.0
 * @date 2025-01-06
 *
 * @copyright Copyright (c) 2025 LX Navigation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <WiFi.h>
#include <HTTPClient.h>
#include <WebSocketsClient.h>
#include "config.h"

//------------------------------------------------------------------------

// Navia Core AP SSID and Password
const char* cSsid       = NAVIA_CORE_SSID;
const char* cPass       = NAVIA_CORE_PASS;

// Navia Core Domain name or IP Address
const char* cCoreAddress = NAVIA_CORE_ADDR;
uint16_t uiRestPort     = 3811;

// REST enpoints
const char* cRestInfo   = "/v1/info";
const char* cRestLogin  = "/v1/user/login";
const char* cRestCmd    = "/v1/command";

// Websocket route
uint16_t  uiWsPort      = 3822;
const char* cWsRoute    = "/ws";

// Authorization token holder
String sToken           = "";

// WebSocket client handler
WebSocketsClient webSocket;

//------------------------------------------------------------------------
//----------------------------- REST METHODS -----------------------------
//------------------------------------------------------------------------

void getInfo()
{
    HTTPClient http;

    // Construct server endpoint url
    String sServerPath = "http://" + String(cCoreAddress) + ":" + String(uiRestPort) + String(cRestInfo);

    Serial.print("[REST] Requesting: GET ");
    Serial.println(sServerPath);

    http.begin(sServerPath.c_str());

    // Send HTTP GET request
    int iHttpResponseCode = http.GET();

    // Print response
    if (iHttpResponseCode > 0)
    {
        Serial.print("[REST] HTTP Response code: ");
        Serial.println(iHttpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
    }
    else {
        Serial.print("[REST] Error code: ");
        Serial.println(iHttpResponseCode);
    }
    // Free resources
    http.end();
}

//------------------------------------------------------------------------

String extractToken(String input)
{
  int index = input.indexOf("\"devaToken\":\"");
  if (index == -1) {
    return ""; // Not found
  }

  index += 13; // Move past the key and quotes (length of "\"devaToken\":\"")
  int endIndex = input.indexOf("\"", index); // Find the next quote

  if (endIndex == -1) {
    return ""; // Invalid format
  }

  return input.substring(index, endIndex);
}

//------------------------------------------------------------------------

void postLogin()
{
    WiFiClient client;
    HTTPClient http;

    // Construct server endpoint url
    String sServerPath = "http://" + String(cCoreAddress) + ":" + String(uiRestPort) + String(cRestLogin);

    Serial.print("[REST] Requesting: POST ");
    Serial.println(sServerPath);

    http.begin(client, sServerPath.c_str());

    // Specify content-type header
    http.addHeader("Content-Type", "application/json");
    // Send HTTP POST request with given json data
    int iHttpResponseCode = http.POST("{\"action\":\"login\", \"email\":\"test@test.si\", \"password\":\"12345678\", \"main_pilot\":true}");

    // Print response
    if (iHttpResponseCode > 0)
    {
        Serial.print("[REST] HTTP Response code: ");
        Serial.println(iHttpResponseCode);
        String payload = http.getString();
        Serial.println(payload);

        // Parse out authorization token
        sToken = extractToken(payload);
    }
    else {
        Serial.print("[REST] Error code: ");
        Serial.println(iHttpResponseCode);
    }

    // Free resources
    http.end();
}

//------------------------------------------------------------------------

void postCommand()
{
    WiFiClient client;
    HTTPClient http;

    // Construct server endpoint url
    String sServerPath = "http://" + String(cCoreAddress) + ":" + String(uiRestPort) + String(cRestCmd);

    Serial.print("[REST] Requesting: POST ");
    Serial.println(sServerPath);

    http.begin(client, sServerPath.c_str());

    // Set Authorization token
    http.setAuthorizationType("Bearer");
    http.setAuthorization(sToken.c_str());

    // Specify content-type header
    http.addHeader("Content-Type", "application/json");
    // Send HTTP POST request with given json data
    int iHttpResponseCode = http.POST("{\"qnh\":1013.2}");

    // Print response
    if (iHttpResponseCode > 0)
    {
        Serial.print("[REST] HTTP Response code: ");
        Serial.println(iHttpResponseCode);
    }
    else {
        Serial.print("[REST] Error code: ");
        Serial.println(iHttpResponseCode);
    }

    // Free resources
    http.end();
}

//------------------------------------------------------------------------
//-------------------------- WEBSOCKET METHODS ---------------------------
//------------------------------------------------------------------------

void setupWebSocket()
{
    Serial.println("Setting up WebSocket client");

    // server address, port and URL
    webSocket.begin(cCoreAddress, uiWsPort, cWsRoute);

    // event handler
    webSocket.onEvent(webSocketEvent);

    // try every 5000 again if connection has failed
    webSocket.setReconnectInterval(5000);
}

//------------------------------------------------------------------------

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length)
{
    switch(type) {
        case WStype_DISCONNECTED:
            Serial.printf("[WSCK] Disconnected!\n");
            break;

        case WStype_CONNECTED:
            Serial.printf("[WSCK] Connected to url: %s\n", payload);
            // subscribe to flight data when Connected
            webSocket.sendTXT("{\"subscribe\":\"flight data\", \"ids\":[\"baro correction\"]}");
            break;

        case WStype_TEXT:
            Serial.printf("[WSCK] Received text: %s\n", payload);
            break;

        case WStype_BIN:
            Serial.printf("[WSCK] Received binary length: %u\n", length);
            break;

        case WStype_ERROR:
        case WStype_FRAGMENT_TEXT_START:
        case WStype_FRAGMENT_BIN_START:
        case WStype_FRAGMENT:
        case WStype_FRAGMENT_FIN:
            break;
    }
}

//------------------------------------------------------------------------
//---------------------------- COMMON METHODS ----------------------------
//------------------------------------------------------------------------

void setup()
{
    Serial.begin(115200);

    Serial.println("Navia Open Example has taken off!");

    WiFi.begin(cSsid, cPass);
    Serial.print("Connecting to network: ");
    Serial.println(cSsid);

    while(WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.print("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());

    setupWebSocket();
}

//------------------------------------------------------------------------

void loop()
{
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED)
    {
        // Request Info
        getInfo();

        // User Login
        postLogin();

        // Set QNH
        postCommand();

        // Handle websocket
        while(true) {
            webSocket.loop();
        }
    }
    else
    {
        Serial.println("WiFi Disconnected");
    }
}

//------------------------------------------------------------------------
