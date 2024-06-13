#include "dashboard.h"

String encryptMode(int eC){
  String Return = "";  
  if (eC == 2){Return = "WPA";}
  if (eC == 5){Return = "WEP";}
  if (eC == 4){Return = "WPA2";}
  if (eC == 7){Return = "None";}
  if ((eC != 2) && (eC != 5) && (eC != 4) && (eC != 7)){Return = "????";}
  return Return;
}

String signalColor(int strength){
  String color = "";
  if (strength >= -70){color = "green;";}
  if (strength <= -70){color = "yellow;";}
  if (strength <= -80){color = "red;";}
  return color;
}

String dashboard(String indexLang, String favicon, String currentSSID, String allPass, int numNetworks, bool validation){
  String acColor = "#ff0003"; //defines the color of the accent stripes on the <h1> lines on the dashboard

  String _html = "<html><head>";
  _html += favicon;
  
  _html += "<meta name='viewport' content='initial-scale=1.0, width=device-width'>";
  _html += "<title> Captive Portal Dashboard</title>";
  _html += "<style>";
  _html += "body {";
  _html += "background: #36393e;";
  _html += "color: #bfbfbf;";
  _html += "font-family: sans-serif;";
  _html += "margin: 0; }";
  
  _html += ".content {max-width: 800px;margin: auto;}";

  _html += ".button-container {display: flex;";
  _html += "align-items: flex-end;";
  _html += "justify-content: space-between;";
  _html += "align-content: center;";
  _html += "flex-wrap: nowrap;";
  _html += "flex-direction: row;";
  _html += "}";
  
  _html += "table {border-collapse: collapse;";
  _html += "width: 100%;";
  _html += "min-width: 400px;";
  _html += "margin-bottom: 2em;";
  _html += "}";
  
  _html += "th, td {";
  _html += "padding: 10px 6px;";
  _html += "text-align: left;";
  _html += "border-bottom: 1px solid #5d5d5d;";
  _html += "}";

  _html += "td, th {text-align: center;}";
  
  _html += "button, input {";
  _html += "display: inline-block;";
  _html += "height: 42px;";
  _html += "padding: 0 30 0 30px;";
  _html += "color:#fff;";
  _html += "text-align: center;";
  _html += "font-size: 11px;";
  _html += "font-weight: 600;";
  _html += "line-height: 38px;";
  _html += "letter-spacing: .1rem;";
  _html += "text-decoration: none;";
  _html += "white-space: nowrap;";
  _html += "background: #2f3136;";
  _html += "border-radius: 4px;";
  _html += "border: none;";
  _html += "cursor: pointer;";
  _html += "box-sizing: border-box;";
  _html += "margin-right: 10px; }";

  _html += "input[type=submit]{ text-transform: uppercase; text-align: left; }";
  _html += "button{ text-transform: uppercase; }";

  _html += "select {";
  _html += "height: 42px;";
  _html += "padding: 6px 10px;";
  _html += "background-color: #2f3136;";
  _html += "border-radius: 4px;";
  _html += "box-shadow: none;";
  _html += "box-sizing: border-box;";
  _html += "color: #d4d4d4;";
  _html += "text-align: center;";
  _html += "border: 0;";
  _html += "margin-right: 20px;";
  _html += "appearance: none;";
  _html += "}";

  _html += "button:hover, input:hover, select:hover {background: #42444a;}";
  
  _html += "h1 {";
  _html += "font-size: 1.7rem;";
  _html += "margin-top: 1rem;";
  _html += "margin-bottom: 1rem;";
  _html += "background: #2f3136;";
  _html += "color: #bfbfbb;";
  _html += "padding: 0.2em 1em;";
  _html += "padding-left: 1rem;";
  _html += "border-radius: 3px;";
  _html += "border-left: solid "+ acColor +" 8px;";
  _html += "font-weight: 100;";
  _html += "padding-left: 9px; }";

  _html += "input[type=number] { -moz-appearance: textfield; }";
  _html += "input::-webkit-outer-spin-button, input::-webkit-inner-spin-button { -webkit-appearance: none; margin: 0; }";

  _html += ".spacer {border: 0; border-top: 1px solid #5d5d5d;";

  _html += ".hrefLink, a, a:link, a:visited {color: red; text-decoration: none;}";
  
  _html += "</style></head>";
  
  _html += "<body>";
  _html += "<div class='content'>";
  _html += "<h1 style='padding: 0.2em 0.1em; text-align: center; font-size: 2.5rem;'>Captive Portal Dashboard</h1>";

  _html += "<p><span style='color: red;'>INFO:<br></span><span>- This project is a proof of concept only for testing and educational purposes<br>- Use it only against your own networks and devices!";
  _html += "<br>- Credits to: Spacehuhn, adamff-dev, sankethj and justcallmekoko their projects helped me understand things and without the knowlege that I have gained form their projects this would probably wouldn't work<br>- While deauting all sites that the esp host will be unresponsive<br>- Made by HEINZGUENTER</span></p>";

  _html += "<div>";
  _html += "<h1>Settings</h1>";

  _html += "<div class='button-container'>";
  _html += "<h2>Language: "+ indexLang +"</h2>";
  _html += "<form action='/language' methode='post'><select name='lang'>";
  _html += "<option value='' disabled selected>Choose</option><option value='DE'>DE</option><option value='EN'>EN</option>";
  _html += "</select><input type=submit value='Change'></form>";
  _html += "</div><hr class='spacer'>";

  //SSID part on the website  
  _html += "<div class='button-container'>";
  _html += "<h2>TargetSSID: "+ currentSSID +"</h2>";
  _html += "<form action='/ssid'><input type=text placeholder='Target SSID' name=ssid></input><input type=submit value='Change SSID'></form>";
  _html += "</div></div>";

  _html += "<hr class='spacer'>";
  _html += "<div class='button-container'>";

  //check and display if validation is turned on
  if (validation == true){ _html += "<h2>Password validation: True</h2>"; } 
  else { _html += "<h2>Password validation: False</h2>"; }

  _html += "<form action='/validate' methode='post'>";
  _html += "<select name='validate'>";
  _html += "<option value='' disabled='' selected=''>Choose</option>";
  _html += "<option value='on'>On</option>";
  _html += "<option value='off'>Off</option>";
  _html += "</select>";
  _html += "<input type='submit' value='Enter'>";
  _html += "</form>";
  _html += "</div>";

  _html += "<div class='button-container'>";
  _html += "<h1 style='padding-right: 7rem;'>Captured Passwords</h1>";
  _html += "<form style='display:inline-block;' method='post' action='/clearPass'><button style='display:inline-block;'>Delete Passwords</button></form>";
  _html += "<form style='display:inline-block;' method='post' action='/dashboard?led_off'><button style='display:inline-block;'>Disable LED</button></form>";
  _html += "</div>";
 
  //adds passwords to the table if one or more was entered
  if (allPass != "") {
    _html += "<table style='text-align: center;'><tbody><tr><th>Password</th><th>Seconds after Bootup</th></tr>";
    
    //add passwords from the storage
    _html += allPass;

    _html += "</tbody></table>";
  } else { _html += "<p><b>No Passwords Captured yet!</b></p>"; }

  //Deauther part on the website
  _html += "<div><h1>Deauther</h1>";
  _html += "<div class='button-container'>";
  _html += "<h2>Target:</h2>";
  _html += "<form action='/deauth' methode='post'>";
  _html += "<select id='target' name='target'>";
  _html += "<option value='' disabled='' selected=''>Choose</option>";

  for (int i = 0; i < numNetworks; i++) { //adds an selection entry for every network
     _html += "<option value='";
     _html += i;
     _html += "'>" + WiFi.SSID(i) + "</option>";
  }
  _html += "</select>";
  
  _html += "<input type='number' min='1' id='duration' name='duration' placeholder='duration(seconds)'>";
  _html += "<input type='submit' value='Deauth'>";
  _html += "</form></div></div></div>";

  _html += "<div class='content'><div class='button-container'>";
  _html += "<h1 style='padding-right: 300px;'>Nearby Networks</h1>";
  _html += "<form style='display:inline-block;' method='post' action='/dashboard'><button onclick='scan()' style='display:inline-block;'>Scan</button></form>";
 
  _html += "</div>";
  _html += "<table><tbody><tr><th>SSID</th><th>Mac Adress</th><th>Channel</th><th>Signal Strength</th><th>Encryption</th></tr>";
  
  for (int i = 0; i < numNetworks; i++) { //adds a table entry for every network
    _html += "<tr><td>" + WiFi.SSID(i) + "</td><td id='mac"+ i +"'>" + WiFi.BSSIDstr(i) + "</td><td id='channel"+ i +"'>" + WiFi.channel(i) + "</td><td style='color: " + signalColor(WiFi.RSSI(i)) + "'>" + WiFi.RSSI(i) + "</td><td>" + encryptMode(WiFi.encryptionType(i)) + "</td></tr>";
  }
  
  _html += "</tbody></table></div></body></html>";

  return _html;
}