#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#ifndef DASHBOARD_H
#define DASHBOARD_H

extern ESP8266WebServer webServer;

String signalColor(const int strength);
String encryptMode(const int eC);
String dashboard(const String& indexLang, const String& favicon, const String& currentSSID, const String& allPass, const int numNetworks, const bool validation);

#endif
