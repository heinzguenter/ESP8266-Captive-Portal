# ESP8266 Captive Portal version 2.3

## Introduction
This captive portal provides multiple languages, a dashboard to view and change all settings, password validation, a function to send validatet credentials to a webhook(for example: [discord webhook](https://support.discord.com/hc/en-us/articles/228383668-Intro-to-Webhooks) or [webhook.site](https://webhook.site)) and a deauthing function(only in the Dual ESP version). This project is a proof of concept for testing and educational purposes. 
Use it only against networks and devices that you have permission to! If you have any questions or suggestions just create an issue or DM me on Discord under the username: heinzguenter.

Currently Supportet languages: English and German.

Credits: [Spacehuhn](https://github.com/SpacehuhnTech) for inspiration on the Design of the dashboard, [adamff-dev](https://github.com/adamff-dev) for the idea and inspiration for the design of the captive portal pages, [sankethj](https://github.com/sankethj) for inspiration for parts of the code and [justcallmekoko](https://github.com/justcallmekoko) without theire projects this would probably not exist.

## Ho to use
1. To use the captive portal, wire the two ESPs together on a breadboard according to the [schematics](https://github.com/heinzguenter/ESP8266-Captive-Portal/blob/main/src/schematics.svg)(The led and the button are optional)
2. Clone or Download the repository and open the master and slave .ino files in the arduino ide
3. Plug in your master ESP and upload the [master script](https://github.com/heinzguenter/ESP8266-Captive-Portal/blob/main/master/master.ino)
4. Then upload the [slave script](https://github.com/heinzguenter/ESP8266-Captive-Portal/blob/main/slave/slave.ino) to your slave ESP8266(make sure you use [spacehuhns custom boards package](https://deauther.com/docs/diy/installation-arduino))
5. Open the [dashboard](192.168.1.1/dashboard)(192.168.178.1/dashboard) and configure the SSID
6. To use the serial output plug the master into your pc and use the serial monitor of the arduino ide

## Screenshots

<h3>192.168.1.1/ Here the User gets informed that there is an "error" and puts in there Password.</h3>
<img src="https://raw.githubusercontent.com/heinzguenter/ESP8266-Captive-Portal/main/src/indexScreenshot.png" title="192.168.1.1/">

<h3>192.168.1.1/restarting This page is displayed after a Password is submitted</h3>
<img src="https://raw.githubusercontent.com/heinzguenter/ESP8266-Captive-Portal/main/src/restartingScreenshot.png" title="192.168.1.1/restarting">

<h3>192.168.1.1/incorrectPass This page is displayed after you enter a wrong password</h3>
<img src="https://raw.githubusercontent.com/heinzguenter/ESP8266-Captive-Portal/main/src/wrongPassScreenshot.png" title="192.168.1.1/restarting">

<h3>192.168.1.1/dashboard Here you can change the Language, SSID and view your captured Passwords</h3>
<img src="https://raw.githubusercontent.com/heinzguenter/ESP8266-Captive-Portal/main/src/dashboardScreenshot.png" title="192.168.1.1/dashboard">

<h3>Logs of every action are printed on the serial connection</h3>
<img src="https://raw.githubusercontent.com/heinzguenter/ESP8266-Captive-Portal/main/src/serialInterfaceScreenshot.png" title="SerialLog">
