#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#ifndef DASHBOARD_H
#define DASHBOARD_H

extern ESP8266WebServer webServer;

const char dashboardCSS[] PROGMEM = 
  "* {"
    "font-family: monospace;"
    "color: #bfbfbb;"
    "margin: 0;"
    "font-size: 18px;"
  "}"

  "body {" 
    "background: #36393e;"
  "}"

  ".button-container {"
    "display: flex;"
    "justify-content: space-between;"
    "align-content: center;"
    "align-items: center;"
    "flex-wrap: nowrap;"
    "flex-direction: row;"
  "}"

  ".button-container h1 {" 
    "width: 70%;"
  "}"

  "table {"
    "border-collapse: collapse;"
    "width: 100%;"
    "min-width: 20.833vw;"
    "margin-bottom: 32px;"
  "}"

  "form { display: flex; }"

  "th, td {"
    "padding: 10px 6px;"
    "border-bottom: 1px solid #5d5d5d;"
    "font-size: 17px;"
    "text-align: center;"
  "}"

  "button, input, select, #pages li a {"
    "height: 42px;"
    "padding: 0px 25px;"
    "text-align: center;"
    "font-size: 13px;"
    "font-weight: 600;"
    "background: #2f3136;"
    "border-radius: 4px;"
    "border: none;"
    "cursor: pointer;"
    "margin-right: 1.042vw;"
    "text-decoration: none;"
  "}"

  "input[type=submit], button { text-transform: uppercase; }"

  "select {"
    "font-weight: 600;"
    "padding: 6px 20px;"
    "background-color: #2f3136;"
    "border-radius: 4px;"
    "text-align: center;"
    "border: 0;"
    "margin-right: 20px;"
    "cursor: pointer;"
    "font-size: 13px;"
    "appearance: none;"
  "}"

  "button:hover, input:hover, select:hover { background: #42444a; }"

  "h1 {"
    "font-size: 32px;"
    "margin: 7px 10px 7px 0;"
    "background: #2f3136;"
    "padding: 3px 16px;"
    "border-radius: 3px;"
    "border-left: solid #ff0003 8px;"
    "padding-left: 12px;"
  "}"

  "h2 {"
    "font-size: 24px;"
    "margin: 18px;"
  "}"

  "#pages {"
    "list-style-type: none;"
    "margin: 0;"
    "padding: 0;"
    "display: flex;"
    "flex-direction: row;"
    "justify-content: center;"
  "}"

  "#pages li {"
    "margin: 10px 10px 10px 0;"
    "display: inline-block;"
  "}"

  "#pages li a {"
    "padding: 19px;"
  "}"

  "input[type=number] { -moz-appearance: textfield; }"

  "input::-webkit-outer-spin-button, input::-webkit-inner-spin-button { -webkit-appearance: none; margin: 0; }"

  ".spacer { border: 0; border-top: 1px solid #5d5d5d; }"

  ".content { max-width: 62.5vw; margin: auto; }"

  "@media only screen and (max-width: 1250px) { .content { max-width: 98%; }";

String signalColor(const int strength);
String encryptMode(const int eC);
String indexDashboard(const String& favicon, const String& CSS, const String& allPass, const int numNetworks, const bool deauthing);
String settingsDashboard(const String& indexLang, const String& favicon, const String& dashboardCSS, const String& currentSSID, const bool validation, const String& webhookurl);

#endif
