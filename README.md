# Smart AC Controller - Human Detection Based Automation
 
This project helps you automatically control your air conditioner based on whether someone is in the room or not. If a person is present, the AC stays on. If the room is empty, the AC turns off but kicks on briefly every 15 minutes just to keep the temperature from going wild. You can also control everything remotely from a web page on your phone or laptop.
 
---
 
## What This System Does
 
- Detects humans in a room using a PIR motion sensor
- Turns the AC on automatically when someone walks in
- Turns the AC off when no one is detected for 5 minutes (adjustable)
- While the room is empty, pulses the AC on for 1 minute every 15 minutes to maintain temperature
- Lets you override everything from a web dashboard on your local Wi-Fi network
- Logs events so you can see what happened and when
---
 
## Why These Components Were Chosen
 
Everything in this list was picked to be affordable, widely available, and beginner-friendly. You can find all of these on Amazon, AliExpress, or your local electronics shop.
 
### Microcontroller - ESP8266 NodeMCU (around 3 to 5 USD)
 
This little board has Wi-Fi built in, which means you do not need a separate Wi-Fi module. It runs on 3.3V, has enough GPIO pins for this project, and there is a massive community online to help you if you get stuck. The Arduino IDE supports it fully, so you write code just like you would for a regular Arduino.
 
### Human Detection - HC-SR501 PIR Sensor (around 1 to 2 USD)
 
PIR stands for Passive Infrared. It detects the heat that humans and animals give off. When something warm moves in its field of view, it sends a signal to the microcontroller. It covers about a 120 degree cone and works reliably up to 7 meters. It has two small potentiometers on the back to adjust sensitivity and how long it stays triggered. Very cheap and very reliable for indoor use.
 
### Relay Module - 5V Single Channel Relay (around 1 to 2 USD)
 
The relay is basically a switch that can be controlled electrically. Your ESP8266 tells the relay to open or close, and the relay then physically connects or disconnects the mains power going to the AC. The relay module usually comes with an indicator LED which is helpful when you are testing. Make sure you get one rated for at least 10A / 250VAC. A 2-ton AC draws around 8 to 10 amps so this is the minimum safe rating. If you want extra safety margin, get a 16A rated relay.
 
### Power Supply - HLK-PM01 or USB 5V Adapter (around 2 to 4 USD)
 
The HLK-PM01 is a tiny module that plugs into mains power and gives you 5V DC output. It is what powers the whole circuit without needing a separate USB charger plugged into the wall. If you are just testing on a desk, a regular phone charger works fine. For a permanent wall-mounted installation, the HLK module is cleaner.
 
### Optional Temperature Sensor - DHT11 or DHT22 (1 to 3 USD)
 
Not required, but nice to have. The DHT11 measures temperature and humidity and sends the readings to your web dashboard. This lets you see if the AC is actually keeping the room cool. DHT22 is a bit more accurate if you want to spend slightly more.
 
---
 
## Parts List and Estimated Cost
 
| Component                        | Quantity | Estimated Cost (USD) |
|----------------------------------|----------|----------------------|
| ESP8266 NodeMCU v3               | 1        | 3 - 5                |
| HC-SR501 PIR Sensor              | 1        | 1 - 2                |
| 5V Relay Module (10A / 250VAC)   | 1        | 1 - 2                |
| HLK-PM01 5V Power Module         | 1        | 2 - 4                |
| DHT11 Temperature Sensor         | 1        | 1 - 2                |
| Jumper wires                     | 10+      | 1                    |
| Small project box / enclosure    | 1        | 2 - 4                |
| Terminal blocks (optional)       | 3        | 1                    |
| **Total**                        |          | **12 - 21**          |
 
---
 
## Wiring Diagram (described in words)
 
### PIR Sensor to ESP8266
- PIR VCC pin goes to the 5V pin on NodeMCU
- PIR GND pin goes to GND on NodeMCU
- PIR OUT pin goes to D2 (GPIO4) on NodeMCU
### DHT11 to ESP8266
- DHT11 VCC goes to 3.3V on NodeMCU
- DHT11 GND goes to GND
- DHT11 DATA goes to D4 (GPIO2) on NodeMCU
### Relay Module to ESP8266
- Relay VCC goes to 5V (the Vin pin on NodeMCU, which passes through USB 5V)
- Relay GND goes to GND
- Relay IN (signal) goes to D1 (GPIO5) on NodeMCU
### Relay to AC Power
This part involves mains electricity. Please be careful. If you are not comfortable working with 230V / 120V wiring, ask an electrician to help with just this last step.
 
- Cut one wire of the AC power cord (the Live / Line wire, usually brown or red)
- Connect one end to the COM terminal on the relay
- Connect the other end to the NO (Normally Open) terminal on the relay
- When the relay closes, power flows. When it opens, the AC is off.
---
 
## Software Setup
 
### Step 1 - Install the Arduino IDE
 
Download it from arduino.cc if you do not have it. The free version is fine.
 
### Step 2 - Add ESP8266 Board Support
 
Open Arduino IDE, go to File > Preferences, and paste this URL into the "Additional Board Manager URLs" box:
 
```
http://arduino.esp8266.com/stable/package_esp8266com_index.json
```
 
Then go to Tools > Board > Board Manager, search for "esp8266", and install it.
 
### Step 3 - Install Required Libraries
 
Go to Sketch > Include Library > Manage Libraries and install these:
 
- DHT sensor library by Adafruit
- ESP8266WebServer (comes with the esp8266 board package)
- ArduinoJson by Benoit Blanchon
### Step 4 - Configure the Code
 
Open the file called `smart_ac_controller.ino`. Near the top you will find these lines:
 
```cpp
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";
```
 
Change those to match your home Wi-Fi. Also check these settings and adjust if needed:
 
```cpp
const int occupancyTimeout = 300;    // seconds before declaring room empty (5 minutes)
const int maintenanceCycleMin = 15;  // minutes between maintenance cycles when empty
const int maintenanceDurationMin = 1; // how long AC runs during maintenance
```
 
### Step 5 - Upload the Code
 
- Connect the ESP8266 to your computer via USB
- Select the correct board: Tools > Board > ESP8266 Boards > NodeMCU 1.0 (ESP-12E Module)
- Select the correct port under Tools > Port
- Click Upload (the arrow button)
### Step 6 - Find the IP Address
 
Open the Serial Monitor (Tools > Serial Monitor, set baud to 115200). After a few seconds you will see something like:
 
```
Connected to WiFi
IP address: 192.168.1.42
```
 
Type that IP address into your phone or laptop browser to open the web dashboard.
 
---
 
## How the Logic Works
 
1. The PIR sensor watches the room constantly
2. When it detects motion, it sets the room status to "occupied" and turns on the AC relay
3. Every time new motion is detected, a 5-minute countdown timer resets
4. If no motion is detected for 5 full minutes, the room is considered empty and the relay opens (AC turns off)
5. While empty, a separate timer counts up to 15 minutes
6. At the 15-minute mark, the relay closes for 1 minute (AC runs briefly to maintain temperature)
7. Then it turns off and the 15-minute countdown starts over
8. If someone walks back in at any point, the AC goes back to full-time on mode
---
 
## Web Dashboard
 
Once you have the IP address, open it in any browser on the same Wi-Fi network.
 
The dashboard shows:
- Current room status (occupied or empty)
- AC state (on or off)
- Temperature and humidity from the DHT11
- How long until next maintenance cycle (when room is empty)
- Manual override buttons to force AC on or off
Remote activation from outside your home requires setting up port forwarding on your router, which varies by router model. For a simpler remote access option, look into a service like ngrok or Tailscale, both of which are free for personal use.
 
---
 
## Troubleshooting
 
**PIR keeps triggering even when room is empty**
Adjust the sensitivity pot on the back of the HC-SR501 counterclockwise to reduce sensitivity. Sunlight through windows or air vents blowing warm air can trigger it.
 
**AC does not turn on**
Double check the relay wiring. The relay module LED should light up when triggered. Check with a multimeter if you have one.
 
**Cannot connect to web dashboard**
Make sure your phone is on the same Wi-Fi network as the ESP8266. The ESP8266 does not support 5GHz networks, so if your phone is on 5GHz and the ESP is on 2.4GHz they will have different IPs. Connect your phone to the 2.4GHz band.
 
**DHT11 shows wrong readings**
DHT11 can take about a minute to stabilize after power-on. Also make sure you have a 10k ohm pull-up resistor between DATA and VCC if your module does not already include one (most breakout modules do).
 
---
 
## Safety Notes
 
- Do not work on mains wiring while the power is on. Always unplug everything first.
- The relay handles mains voltage. Keep the relay wiring away from the low-voltage ESP8266 wiring.
- Put everything inside an enclosure so nothing is accidentally touched.
- A 2-ton AC can draw significant current. Make sure all wire gauges are rated for the load.
- This is a DIY project and comes with no warranty. Build and use it at your own risk.
---
 
## File Structure
 
```
smart-ac-controller/
  README.md                  - this file
  smart_ac_controller.ino    - Arduino firmware for the ESP8266
  demo.html                  - standalone demo to simulate the system in a browser
  web_dashboard/
    index.html               - the dashboard served by the ESP8266
```
 
---
 
## License
 
MIT License. Use this however you want, personal or commercial.
 
