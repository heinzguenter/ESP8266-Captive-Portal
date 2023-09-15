# Captive Portal Documentation

## Introduction
This Captive Portal Proviedes Multiple Languages and a Dashboard to view and change all settings in one place. This project is a proof of concept for testing and educational purposes. 
Use it only against your own networks and devices! If you have any questions or suggestions just create an issue or DM me on Discord under the username heinzguenter.

Credits: [Spacehuhn](https://github.com/SpacehuhnTech) for inspiration on the Design of the dashboard, [adamff-dev](https://github.com/adamff-dev) for the idea and inspiration for parts of the code, and [sankethj](https://github.com/sankethj) also for inspiration for parts of the code.

## Libraries Used
1. `ESP8266WiFi.h`: Provides functions for connecting to Wi-Fi networks.
2. `DNSServer.h`: Implements a DNS server for DNS spoofing.
3. `ESP8266WebServer.h`: Allows you to create a web server for handling HTTP requests.
4. `EEPROM.h`: Provides functions for reading and writing data to the ESP8266's EEPROM.

## Constants and Variables
- `SSID_NAME`: Default SSID name for the captive portal.
- `DNS_PORT`: Port for the DNS server.
- `APIP`: Gateway IP address.
- `allPass`: String to store captured passwords.
- `currentSSID`: Current SSID of the Wi-Fi network.
- `networks`: Result of the network scan.
- `indexLang`: Language setting for the index page.
- `initialCheckLocation`: Location in EEPROM to check if the ESP is running for the first time.
- `passStart`: Starting location in EEPROM to save passwords.
- `passEnd`: Ending location in EEPROM to save passwords.
- `bootTime`, `lastActivity`, `lastTick`, `tickCtr`: Variables for tracking time.
- `dnsServer`: Instance of the DNS server.
- `webServer`: Instance of the web server.
- `favicon`: Base64-encoded favicon data.

## Functions
1. `input(String argName)`: Helper function to get input values from HTTP request parameters.
2. `indexCSS()`: Generates CSS for the index page.
3. `Index()`: Handles the index page request.
4. `handlePost()`: Handles the POST request from the index page for password submission.
5. `postSSID(String postedSSID)`: Handles changing the SSID of the Wi-Fi network.
6. `LED(bool light)`: Controls the built-in LED on the ESP8266 module.
7. `encryptMode(int eC)`: Converts encryption mode to a human-readable string.
8. `signalColor(int strength)`: Determines the color based on signal strength for styling.
9. `clearPass()`: Clears captured passwords.
10. `handleLang()`: Handles changing the language setting.
11. `handleDashboard()`: Handles the dashboard page request.

## Setup Function
- Initializes serial communication.
- Checks if the ESP8266 is running for the first time and initializes EEPROM if needed.
- Reads the stored SSID and passwords from EEPROM.
- Sets up Wi-Fi in SoftAP mode.
- Initializes DNS server, web server, and routes.

## Usage
1. Upload this script to your ESP8266 device.
2. Connect to the ESP8266's Wi-Fi network (default SSID is "Portal").
3. Open a web browser, and type in "192.168.1.1/dashboard" to get to the dashboard or "192.168.1.1" for the page that a normal user would see.
4. There you can view the captured passwords and edit the SSID and other settings.