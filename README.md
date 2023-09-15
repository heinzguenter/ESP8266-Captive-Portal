# Captive Portal
 
# Captive Portal Arduino Script Documentation

## Introduction
This Arduino script creates a captive portal using an ESP8266 module. A captive portal is a web page that is displayed to users when they connect to a Wi-Fi network. It typically requires users to provide some information or perform an action, such as agreeing to terms and conditions or entering a password, before they can access the internet.

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
- Enables the built-in LED.

## Usage
1. Upload this script to your ESP8266 device.
2. Connect to the ESP8266's Wi-Fi network (default SSID is "Portal").
3. Open a web browser, and you should be redirected to the captive portal index page.
4. Enter the Wi-Fi password in the input field and click "Router neu starten" or "Restart Router."
5. The ESP8266 will restart, and most problems should be resolved.
6. Access the dashboard page to view captured passwords and nearby networks.

## Note
- The ESP8266 stores captured passwords in EEPROM.
- You can change the default SSID name and customize the web page content by modifying the script.
- Use this script responsibly and comply with applicable laws and regulations regarding network usage and privacy.
