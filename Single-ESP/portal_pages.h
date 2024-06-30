#include "Arduino.h"
#ifndef PORTAL_PAGES_H
#define PORTAL_PAGES_H

String indexCSS();
String wrongPass(const String& indexLang, const String& favicon, const String&currentSSID);
String Index(const String& indexLang, const String& favicon, const String& currentSSID);
String Fixing(const String& indexLang, const String& favicon, const String& currentSSID);

#endif
