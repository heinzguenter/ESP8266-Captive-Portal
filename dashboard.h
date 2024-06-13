#include "Arduino.h"
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#ifndef DASHBOARD_H
#define DASHBOARD_H

extern ESP8266WebServer webServer;

String signalColor(int strength);
String encryptMode(int eC);
String dashboard(String indexLang, String favicon, String currentSSID, String allPass, int numNetworks, bool validation);

#endif
