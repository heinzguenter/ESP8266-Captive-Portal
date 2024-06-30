# ESP8266 Captive Portal

## Introduction
This Captive Portal Provides Multiple Languages, a Dashboard to view and change all settings in one place, password validation and a deauthing function(only in the Dual ESP version). This project is a proof of concept for testing and educational purposes. 
Use it only against networks and devices that you have permission to! If you have any questions or suggestions just create an issue or DM me on Discord under the username: heinzguenter.

Currently Supportet languages: English and German.

Credits: [Spacehuhn](https://github.com/SpacehuhnTech) for inspiration on the Design of the dashboard, [adamff-dev](https://github.com/adamff-dev) for the idea and inspiration for the design of the captive portal pages, [sankethj](https://github.com/sankethj) for inspiration for parts of the code and [justcallmekoko](https://github.com/justcallmekoko) without theire projects this would probably not exist.

## Versions
The Captive Portal comes in two versions, one for a single ESP8266 and one for dual ESP8266's. The main difference is that the singe version doesnt have a deauther(because while deauthing the Captive Portal goes unressponsive and is there for useles) and the password validation feature is a bit worse. To use the dual ESP8266 version wire them according to the [schematics](https://github.com/heinzguenter/ESP8266-Captive-Portal/main/src/schematics.svg)(The led and the button is optional) and upload the [master script](https://github.com/heinzguenter/ESP8266-Captive-Portal/blob/main/Dual-ESP/master/master.ino) to your master ESP8266 and the [slave script](https://github.com/heinzguenter/ESP8266-Captive-Portal/blob/main/Dual-ESP/slave/slave.ino) to your slave ESP8266.

## Screenshots

<h3>192.168.1.1/ Here the User gets informed that there is an "error" and puts in there Password.</h3>
<img width="1278px" src="https://raw.githubusercontent.com/heinzguenter/ESP8266-Captive-Portal/main/src/index-screenshot.png" title="192.168.1.1/">

<h3>192.168.1.1/restarting This page is displayed after a Password is submitted</h3>
<img width="1278px" src="https://raw.githubusercontent.com/heinzguenter/ESP8266-Captive-Portal/main/src/restarting-screenshot.png" title="192.168.1.1/restarting">

<h3>192.168.1.1/dashboard Here you can change the Language, SSID and view your captured Passwords</h3>
<img width="1261" src="https://raw.githubusercontent.com/heinzguenter/ESP8266-Captive-Portal/main/src/dashboard-screenshot.png" title="192.168.1.1/dashboard">

<h3>Logs of every action are printed on the serial connection</h3>
<img width="410" src="https://raw.githubusercontent.com/heinzguenter/ESP8266-Captive-Portal/main/src/Serial-Interface-screenshot.png" title="SerialLog">

## Usage
1. Download and add Spacehuhn's custom board to your IDE(you can follow [this guide](https://deauther.com/docs/diy/installation-arduino/))
2. Change or add things if you want
3. Connect your ESP8266 to your PC and chose the right port and Board for you
4. Press the upload button and wait for it to complete
5. Open a web browser, and type in "192.168.1.1/dashboard" to get to the dashboard
	 or "192.168.1.1" for the page that a normal user would see.
