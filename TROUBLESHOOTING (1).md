# Troubleshooting Guide - Smart AC Controller

This guide covers the most common problems people run into when building this project
and how to solve them. Read through the symptom that matches your problem.


---


## The ESP8266 Will Not Upload Code

Problem: Arduino IDE shows an error when you try to upload. Common errors include
"esptool.py failed to connect", "error opening serial port", or "no port found".

Solution 1 - Wrong board selected
    Go to Tools > Board and make sure "NodeMCU 1.0 (ESP-12E Module)" is selected.
    If you have a Wemos D1 Mini, select "LOLIN(Wemos) D1 R2 & mini" instead.

Solution 2 - Wrong port selected
    Go to Tools > Port. On Windows this is something like COM3 or COM5.
    On Mac it is something like /dev/cu.usbserial-1410. On Linux it is /dev/ttyUSB0.
    If no port appears, your USB driver is not installed. See Solution 3.

Solution 3 - USB driver not installed
    NodeMCU boards use either a CH340G or CP2102 USB chip. You can see which one
    you have by looking at the small chip near the USB port.
    CH340G driver: search "CH340G driver download" for your OS
    CP2102 driver: search "CP2102 driver Silicon Labs" for your OS
    Restart the Arduino IDE after installing the driver.

Solution 4 - Board not in flash mode
    Hold down the FLASH button on the NodeMCU, click Upload in Arduino IDE,
    wait until you see "Connecting..." in the status bar, then release the button.
    Some boards need this, some do not.

Solution 5 - Upload speed too fast
    Go to Tools > Upload Speed and change it from 921600 to 115200. Slower is more reliable.


---


## Serial Monitor Shows Garbled Text

Problem: When you open Serial Monitor, you see random symbols and garbage characters.

Solution: Make sure the baud rate in the Serial Monitor dropdown (bottom right of the
Serial Monitor window) is set to 115200. This must match what is in the code.


---


## ESP8266 Will Not Connect to Wi-Fi

Problem: Serial Monitor keeps printing dots but never shows "Connected to WiFi".

Solution 1 - Wrong credentials
    Double check your WIFI_SSID and WIFI_PASSWORD in config.h (or config_secrets.h).
    The SSID and password are case sensitive.

Solution 2 - 5GHz network
    The ESP8266 only supports 2.4GHz Wi-Fi. If your router broadcasts on 5GHz only,
    the ESP8266 cannot connect. Log into your router and make sure 2.4GHz is enabled.
    Your phone will likely connect to 5GHz by default, so the fact that your phone
    connects does not mean the 2.4GHz band is working.

Solution 3 - Router has MAC filtering
    Some routers only allow specific devices. Check your router settings and either
    disable MAC filtering or add the ESP8266's MAC address. The MAC address prints
    in Serial Monitor during startup.

Solution 4 - Wrong country code or channel
    In rare cases, the ESP8266 has trouble with certain Wi-Fi channels. If nothing
    else works, try changing your router to broadcast on channel 6.


---


## Web Dashboard Does Not Load

Problem: You have the IP address but the browser just times out or says "cannot connect".

Solution 1 - Wrong device on network
    Your phone must be on the SAME Wi-Fi network as the ESP8266. If your phone is on
    5GHz and the ESP8266 is on 2.4GHz, they have different IP ranges and cannot talk
    to each other. Connect your phone explicitly to the 2.4GHz band.

Solution 2 - Firewall on the router
    Some routers block devices from communicating with each other. This is called
    "AP isolation" or "client isolation". Check your router settings and disable it.

Solution 3 - Wrong IP address
    The IP address changes if the ESP8266 reconnects to Wi-Fi or if your router
    assigns a new one. Always check the Serial Monitor for the current IP. See the
    Static IP section in config.h to make the IP permanent.

Solution 4 - Port 80 blocked
    If your network admin blocks port 80, change WEB_SERVER_PORT in config.h to
    8080 or another port, then access the dashboard at 192.168.x.x:8080.


---


## PIR Sensor Keeps Triggering When Room Is Empty

Problem: The AC keeps turning on even when no one is in the room.

Solution 1 - Reduce sensitivity
    The small potentiometer on the back of the HC-SR501 labeled Sx (or CDS) controls
    sensitivity. Turn it counterclockwise to reduce the detection range. Try halfway
    or less for a normal-sized room.

Solution 2 - Sunlight or heat sources
    The PIR sensor detects infrared heat. Direct sunlight, a lamp, a heater, a hot
    air vent, or even a warm computer can trigger it. Aim the sensor away from these.

Solution 3 - Wrong trigger mode
    The jumper on the back should be set to H (repeatable trigger). If it is on L,
    the sensor can behave unexpectedly. Check the jumper.

Solution 4 - Sensor needs warm-up time
    After power-on the HC-SR501 needs about 30 to 60 seconds to stabilize. During this
    time it may trigger randomly. This is normal. Just wait a minute before testing.

Solution 5 - Loose wiring
    A loose wire on the signal pin can cause random HIGH signals. Reseat all connections.


---


## PIR Sensor Never Triggers

Problem: Walking in front of the sensor does nothing.

Solution 1 - Wrong wiring
    Check that PIR OUT is connected to D2 (GPIO4) on the NodeMCU and not to any other pin.

Solution 2 - Sensor facing wrong direction
    The white dome must have a clear line of sight to the area you want to monitor.
    Do not mount it inside an enclosure with the dome covered.

Solution 3 - Detection range too low
    Turn the Sx pot clockwise to increase sensitivity.

Solution 4 - Defective sensor
    PIR sensors are cheap and occasionally one arrives dead. Test with a multimeter:
    the OUT pin should go to about 3.3V when triggered and drop to near 0V when idle.
    If it stays at 0V or 3.3V permanently, the sensor is likely dead.


---


## DHT11 Reads Wrong Values or Shows NaN

Problem: Temperature or humidity shows NaN (not a number) or physically impossible values.

Solution 1 - Missing pull-up resistor
    If you are using a bare DHT11 (not a breakout module), you need a 10k ohm resistor
    between the DATA pin and the VCC pin. Most modules have this built in.

Solution 2 - Reading too fast
    The DHT11 needs at least 1 second between readings. The code reads every 5 seconds
    which should be fine, but check the DHT_READ_INTERVAL_MS setting in config.h.

Solution 3 - Wrong pin
    Check that DHT11 DATA is connected to D4 (GPIO2) and matches PIN_DHT in config.h.

Solution 4 - Needs a few minutes to stabilize
    After power-on, the DHT11 can take 1 to 2 minutes before readings are accurate.
    The first few readings may be off slightly. This is normal.

Solution 5 - Using DHT22 but code is set to DHT11
    If you have a DHT22 sensor, change DHTTYPE in config.h from DHT11 to DHT22.


---


## Relay Clicks But AC Does Not Turn On

Problem: You can hear the relay clicking (or see its LED) but the AC stays off.

Solution 1 - Wrong relay terminals
    Make sure you are using COM and NO terminals, not COM and NC. NO means Normally Open,
    which means the circuit is open (disconnected) when the relay is off. When the relay
    closes, COM and NO connect and power flows. NC means Normally Closed and works the opposite.

Solution 2 - Loose mains wires
    Tighten the screw terminals on the relay. Mains wires need a firm connection.

Solution 3 - AC already has power from another path
    If your AC has a mechanical power switch that is in the ON position, and you are
    cutting the live wire before that switch, make sure the switch is also on.

Solution 4 - Relay is wired backwards
    Try swapping which mains wire goes to COM and which goes to NO. The circuit should
    complete either way but sometimes it helps to identify a wiring error.


---


## Relay Does Not Click At All

Problem: No clicking sound, LED on relay module does not light up when it should.

Solution 1 - Wrong pin
    Check that Relay IN is connected to D1 (GPIO5) and matches PIN_RELAY in config.h.

Solution 2 - Relay polarity
    Most relay modules are active LOW (you send LOW to turn them on). The code is
    written for active LOW relays. If yours is active HIGH, set RELAY_ACTIVE_LOW to 0
    in config.h.

Solution 3 - Relay needs more current than NodeMCU can provide
    The NodeMCU GPIO pins can source about 12mA. The relay coil may need more.
    Make sure VCC on the relay goes to the 5V (Vin) pin on the NodeMCU, not the 3.3V pin.
    The Vin pin can supply more current.

Solution 4 - Defective relay module
    Apply 5V directly to the relay VCC and GND from a USB charger, and connect IN to GND
    manually. The relay should click. If it does not, the relay module may be dead.


---


## AC Turns On Then Immediately Off Again

Problem: The AC flickers on and off rapidly.

Solution: This usually means a loose connection on the relay signal wire. Check that
D1 to Relay IN wire is firmly connected at both ends. Also check for any short circuits
near the relay module.


---


## Getting Help

If your problem is not listed here, these resources can help:

    Arduino Forum: forum.arduino.cc
    ESP8266 Community: esp8266.com/viewforum.php?f=6
    GitHub Issues: open an issue on this project's GitHub page with details of your problem

When asking for help, always include:
    1. What your Serial Monitor shows
    2. A photo of your wiring if possible
    3. What you have already tried
