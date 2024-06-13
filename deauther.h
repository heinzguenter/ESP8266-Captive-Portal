#include "Arduino.h"
#include <ESP8266WiFi.h>

#ifndef DEAUTH_H
#define DEAUTH_H

void deauth(int channel, uint8_t bssid[6], int duration);

#endif
