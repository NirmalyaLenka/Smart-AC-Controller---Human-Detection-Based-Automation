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
 
The list below describes the prototype's intended components, not a validated installation. Price estimates and generic relay ratings are not evidence that a part is suitable for an air-conditioner compressor or mains circuit.
 
### Microcontroller - ESP8266 NodeMCU (around 3 to 5 USD)
 
This little board has Wi-Fi built in, which means you do not need a separate Wi-Fi module. It runs on 3.3V, has enough GPIO pins for this project, and there is a massive community online to help you if you get stuck. The Arduino IDE supports it fully, so you write code just like you would for a regular Arduino.
 
### Human Detection - HC-SR501 PIR Sensor (around 1 to 2 USD)
 
PIR stands for Passive Infrared. It detects the heat that humans and animals give off. When something warm moves in its field of view, it sends a signal to the microcontroller. It covers about a 120 degree cone and works reliably up to 7 meters. It has two small potentiometers on the back to adjust sensitivity and how long it stays triggered. Very cheap and very reliable for indoor use.
 
### Switching interface — not validated for mains use

The sketch drives a low-voltage relay input on D1 (GPIO5), active LOW in the current firmware. The repository does not validate a particular relay, its contact rating, isolation, failure mode, or ability to handle compressor inrush. A generic 10 A relay must not be described as a safe minimum for an AC unit. Do not connect this prototype to mains or an air conditioner based on this README. A qualified electrician must select and install a suitable, enclosed switching interface and protection for the actual load and local standards.

### Power Supply - HLK-PM01 or USB 5V Adapter (around 2 to 4 USD)
 
The HLK-PM01 has a mains-voltage input. For development, use USB power for the NodeMCU. Do not treat an exposed AC/DC module as a ready-made permanent supply; any mains-powered installation requires a suitable enclosure, protection, clearances and qualified installation.
 
### Optional Temperature Sensor - DHT11 or DHT22 (1 to 3 USD)
 
Not required, but nice to have. The DHT11 measures temperature and humidity and sends the readings to your web dashboard. This lets you see if the AC is actually keeping the room cool. DHT22 is a bit more accurate if you want to spend slightly more.
 
---
 
## Parts List and Estimated Cost
 
| Component                        | Quantity | Estimated Cost (USD) |
|----------------------------------|----------|----------------------|
| ESP8266 NodeMCU v3               | 1        | 3 - 5                |
| HC-SR501 PIR Sensor              | 1        | 1 - 2                |
| Switching interface selected for the actual AC load by a qualified electrician | 1 | To be determined |
| HLK-PM01 5V Power Module         | 1        | 2 - 4                |
| DHT11 Temperature Sensor         | 1        | 1 - 2                |
| Jumper wires                     | 10+      | 1                    |
| Small project box / enclosure    | 1        | 2 - 4                |
| Terminal blocks (optional)       | 3        | 1                    |
| **Total**                        |          | **12 - 21**          |
 
---
 
## Low-voltage wiring reference

The current sketch uses these NodeMCU ESP8266 pins. Keep the relay's mains/load side disconnected during development.

| Component signal | NodeMCU connection | Notes |
|---|---|---|
| HC-SR501 PIR VCC / GND | VIN/5 V and GND, as supported by the module | Check the exact sensor board's input rating. |
| HC-SR501 PIR OUT | D2 / GPIO4 | Digital motion input. |
| DHT11 VCC / GND | 3.3 V and GND | Match the sensor module's requirements. |
| DHT11 DATA | D4 / GPIO2 | DHT11 in the current sketch. |
| Relay control VCC / GND | Appropriate low-voltage supply and common GND | Verify the chosen module's requirements; do not connect its load terminals to mains during tests. |
| Relay IN | D1 / GPIO5 | Firmware drives LOW for ON and HIGH for OFF. |

See [WIRING.md](WIRING.md) for the low-voltage reference and the mains safety boundary. Do not follow generic step-by-step mains diagrams from older copies of this README.

## Software Setup

### Prerequisites

- Arduino IDE and the ESP8266 Arduino board package
- NodeMCU ESP8266-compatible board
- Arduino libraries: **DHT sensor library by Adafruit** and **ArduinoJson by Benoit Blanchon**. ESP8266WebServer and Wi-Fi support come with the ESP8266 board package.
- USB data cable and Serial Monitor

### 1. Configure the checked-in sketch

Open the root-level smart_ac_controller.ino. The active sketch currently hardcodes Wi-Fi credentials and timing constants near the top:

- ssid and password
- occupancyTimeoutSec = 300 seconds
- maintenanceCycleSec = 900 seconds
- maintenanceDurationSec = 60 seconds
- overrideTimeoutSec = 1800 seconds

Replace the Wi-Fi placeholders only in a local working copy. Do not commit real credentials. The files named config (1).h and config_secrets.h.example are not included by the current sketch, so editing them alone does not change firmware behavior. The sketch uses port 80 directly; the WEB_SERVER_PORT value in config (1).h is not active.

### 2. Upload and inspect logs

1. Select **NodeMCU 1.0 (ESP-12E Module)** or the exact ESP8266 board you have.
2. Select the correct serial port and upload the sketch.
3. Open Serial Monitor at **115200 baud**.
4. Read the assigned IP address from the boot log and open that address from a trusted device on the same LAN.

Do not connect the relay's load terminals to mains during bring-up. Start with USB power and low-voltage sensor/relay-control checks.

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
 
## Web Dashboard and network exposure

The sketch embeds a dashboard page and serves it at the root route. It also provides GET /status and GET /control? action commands (on, off, auto). The control endpoints have no authentication or authorization; anyone who can reach port 80 on the device may be able to change the relay state.

Use only on a trusted local network. Do not configure router port forwarding, expose the controller directly to the internet, or rely on a tunnel as a substitute for authentication. The root index.html file is separate from the HTML embedded in the firmware and is not the file served by the current sketch.

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

- This prototype is not a certified AC power controller.
- Keep the relay's mains/load terminals disconnected during firmware and low-voltage testing.
- Do not assume a relay's printed current rating makes it suitable for compressor inrush or continuous AC operation.
- Any mains switching design must be selected, enclosed, protected and installed by a qualified electrician under applicable local standards.
- Never work on energized mains wiring.
- Do not expose the unauthenticated control endpoints to untrusted networks.
- The code, wiring and hardware were not built or tested during this documentation review.

## Actual repository file structure

The current files are at the repository root; the older folder tree in previous versions of this README is not present.

```text
.
├── smart_ac_controller.ino      # Active ESP8266 sketch; embeds the dashboard HTML
├── index.html                   # Separate HTML file, not served by the current sketch
├── README.md
├── WIRING.md                    # Wiring and safety reference
├── COMPONENTS.md                # Component notes and proposed BOM
├── TROUBLESHOOTING (1).md       # Troubleshooting guide
├── CHANGELOG.md                 # Release notes
├── config (1).h                 # Not included by the active sketch
├── config_secrets.h.example     # Template not included by the active sketch
├── gitignore (2)                # Filename is not recognized by Git as .gitignore
└── LICENSE (2)
```

## File guide and known limitations

- smart_ac_controller.ino — active firmware, hardcoded Wi-Fi placeholders and timing constants, relay control, embedded dashboard, JSON status endpoint and manual-control route.
- WIRING.md — low-voltage signal connections and safety boundary. It is not a mains installation design.
- COMPONENTS.md — proposed parts list; the previous generic relay-rating claims have been corrected to require load-specific validation.
- TROUBLESHOOTING (1).md — upload, Wi-Fi, sensor and relay troubleshooting notes. Treat any mains-related steps as requiring qualified review.
- CHANGELOG.md — intended release history and future ideas.
- config (1).h and config_secrets.h.example — currently not included by the active sketch; changing these files alone does not configure the firmware.
- index.html — standalone HTML file; the active firmware serves a separate embedded HTML string.
- gitignore (2) — a renamed file, not an effective .gitignore. Never put real Wi-Fi credentials in tracked source files.

## License
 
MIT License. Use this however you want, personal or commercial.
 


---

## Skills & technologies

`Arduino/C++` · `C/C++` · `HTML/CSS`


## How it works and verification status

The ESP8266 reads motion from the HC-SR501 PIR on D2/GPIO4, drives a relay-control signal on D1/GPIO5 (LOW = ON in the current sketch), and reads a DHT11 on D4/GPIO2. After 300 seconds without motion it turns the relay off; when the room is empty it runs a 60-second maintenance pulse after each 900-second cycle. Web overrides expire after 1800 seconds.

Important limitations: the active sketch hardcodes Wi-Fi credentials and timing values, does not include the checked-in configuration headers, and exposes unauthenticated HTTP controls on port 80. The repository does not establish that the relay is suitable for AC compressor loads or that the mains design is safe.

Only documentation was changed. No firmware build, network security test, or hardware test was performed.

## Maintenance & Audit

- **Audit date:** 17 November 2026
- **Documentation status:** README reviewed and audit record added on this date.
- **Scope and evidence:** This is a documentation-level maintenance pass. It does not establish that source code compiles, dependencies are current, security has been tested, or hardware/runtime behavior has been verified.
- **Code and tests:** No new functional code changes or test results are claimed by this README update. Run project-specific build, test, and hardware/runtime checks before relying on the project.
- **Maintenance baseline:** Documentation status recorded through 11 November 2026; revisit setup instructions, dependencies, known limitations, and tests when the project changes.

## Contact

Maintained up to 25 November 2026.

If you encounter any problems, please contact [carmodbhai@gmail.com](mailto:carmodbhai@gmail.com).
