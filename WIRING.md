# Wiring Diagram - Smart AC Controller

This file describes all the wiring connections in plain text and ASCII art.
If you are new to electronics, read the "Plain English Connections" section first.
The ASCII diagram is there as a quick visual reference once you understand the basics.


---


## Safety Warning

The low-voltage section (everything involving the ESP8266, sensors, and relay module signal wires)
is completely safe to work on with the power on. These run at 3.3V or 5V, which cannot hurt you.

The HIGH-VOLTAGE section (the wires going from the wall outlet to the AC unit through the relay)
is 230V or 110V mains power. This CAN kill you. Always unplug everything from the wall before
touching those wires. When in doubt, ask an electrician to do only the mains wiring part.


---


## Component Overview

    NodeMCU ESP8266     - the brain. Has Wi-Fi and runs your code.
    HC-SR501 PIR        - motion detector. Outputs HIGH when it sees movement.
    DHT11               - temperature and humidity sensor.
    5V Relay Module     - electrically controlled switch for the AC mains power.
    HLK-PM01 (5V 600mA) - tiny power supply. Converts 230V mains to 5V DC.


---


## ASCII Wiring Diagram (Low Voltage Side)

    +--------------------------+         +-------------------+
    |     HC-SR501 PIR         |         |   DHT11 Sensor    |
    |                          |         |                   |
    |  VCC  GND  OUT           |         |  VCC  GND  DATA   |
    +--+----+----+-------------+         +--+----+----+------+
       |    |    |                          |    |    |
       |    |    +-- D2 (GPIO4)             |    |    +-- D4 (GPIO2)
       |    +------- GND                   |    +------- GND
       +------------ 5V (Vin)              +------------ 3.3V


    +-----------------------------+
    |   5V Relay Module           |
    |                             |
    |  VCC  GND  IN               |
    +--+----+----+----------------+
       |    |    |
       |    |    +-- D1 (GPIO5)
       |    +------- GND
       +------------ 5V (Vin)


    +-----------------------------------------------+
    |               NodeMCU ESP8266                 |
    |                                               |
    |  3.3V  5V(Vin)  GND   D1   D2   D4           |
    +---+------+--------+----+----+----+------------+
        |      |        |    |    |    |
        |      |        |    |    |    +-------> DHT11 DATA
        |      |        |    |    +-----------> PIR OUT
        |      |        |    +----------------> Relay IN
        |      |        +--------------------->
        |      |                              GND (shared across all components)
        |      +------------------------------> PIR VCC, Relay VCC
        +-------------------------------------> DHT11 VCC


---


## ASCII Wiring Diagram (High Voltage / Mains Side)

    WALL SOCKET (230V)
         |
         +--- NEUTRAL (Blue/White) ------+------> AC Unit NEUTRAL (do not cut this wire)
         |                               |
         +--- LIVE (Brown/Red/Black) --> [CUT HERE]
                                          |
                          +---------------+
                          |
                    RELAY COM terminal
                          |
                    RELAY NO terminal  (Normally Open - circuit is open when relay is off)
                          |
                          +---------------> AC Unit LIVE
                          |
                    (When relay closes, mains power flows to AC)


    HLK-PM01 Power Module
         |
         +--- AC Input L --> LIVE from wall (same live wire, before the cut)
         +--- AC Input N --> NEUTRAL from wall
         +--- DC Output + --> 5V pin on NodeMCU (Vin)
         +--- DC Output - --> GND pin on NodeMCU


---


## Plain English Connections

Read this section if the diagram above is confusing.

### Step 1 - Connect the PIR Sensor

The HC-SR501 has three pins. Looking at the front of the sensor (the white dome side),
they are usually labeled VCC, OUT, GND from left to right (check your specific module).

    PIR VCC  ----> NodeMCU 5V (the pin labeled Vin or 5V, not the 3.3V pin)
    PIR GND  ----> NodeMCU GND (any GND pin)
    PIR OUT  ----> NodeMCU D2

After wiring, there are two small potentiometers on the back of the PIR:
    - Left pot  controls sensitivity (how far it detects). Turn clockwise to increase.
    - Right pot controls how long it stays HIGH after detecting motion. Turn fully counterclockwise
      for the shortest hold time, which gives the firmware more control.

The jumper (small plastic cap) on the back should be set to the H position (single trigger mode).


### Step 2 - Connect the DHT11

The DHT11 has either 3 or 4 pins. If it is a bare component (4 pins), pin 1 is VCC and pin 4 is GND.
Pin 2 is data. Pin 3 is not used. Most breakout modules (on a small PCB) only expose 3 pins.

    DHT11 VCC   ----> NodeMCU 3.3V
    DHT11 GND   ----> NodeMCU GND
    DHT11 DATA  ----> NodeMCU D4

If your bare DHT11 (not a breakout module) reads incorrectly, add a 10k ohm resistor between
the DATA pin and the VCC pin. Most breakout boards already have this built in.


### Step 3 - Connect the Relay Module

The relay module has three signal pins on one side (for the ESP8266 to control) and three
screw terminals on the other side (for the mains wiring).

Signal side (low voltage, safe):
    Relay VCC  ----> NodeMCU 5V (Vin pin)
    Relay GND  ----> NodeMCU GND
    Relay IN   ----> NodeMCU D1

Screw terminal side - you will see three terminals labeled COM, NO, NC:
    COM = Common. One mains wire connects here.
    NO  = Normally Open. The other mains wire connects here.
    NC  = Normally Closed. Leave this empty for this project.

When the relay is off (AC off): COM and NO are disconnected. No power flows.
When the relay is on (AC on):   COM and NO are connected. Power flows to the AC.


### Step 4 - Connect the HLK-PM01 Power Supply

This tiny module has four connections, usually labeled on the module itself:

    L (AC Input)   ----> Live wire from your wall outlet
    N (AC Input)   ----> Neutral wire from your wall outlet
    +V (DC Output) ----> NodeMCU Vin (5V pin)
    -V (DC Output) ----> NodeMCU GND

This module gets its input from the same mains connection that feeds the relay.
You can use a terminal block to split the live and neutral wires so both the
HLK-PM01 and the relay COM terminal get mains power.


### Step 5 - Mains Wiring to the AC Unit

ONLY do this step after everything else is working and tested. UNPLUG FROM THE WALL FIRST.

Your AC power cord has at least two wires inside: Live and Neutral.
In most countries: Live is Brown (or Red), Neutral is Blue (or Black or White).
In India: Live is Red, Neutral is Black.

1. Cut the LIVE wire only. Do not cut the Neutral.
2. Strip about 1cm of insulation from both cut ends.
3. Connect one cut end to the COM terminal on the relay.
4. Connect the other cut end to the NO terminal on the relay.
5. The Neutral wire goes straight through uncut to the AC unit.

Now when the relay closes, the live wire is connected and the AC receives full power.
When the relay opens, the AC is completely cut off from the live wire.


---


## Shared Ground

All GND pins in this project are connected together. This is called a common ground.
NodeMCU GND, PIR GND, DHT11 GND, Relay GND, and HLK-PM01 negative output all connect
to the same ground point. You can use a small terminal block or a breadboard for this.


---


## NodeMCU Pin Reference

    Pin Label | GPIO Number | Used For
    ----------|-------------|-------------------------
    D1        | GPIO5       | Relay IN signal
    D2        | GPIO4       | PIR sensor output
    D4        | GPIO2       | DHT11 data
    Vin       | -           | 5V output (from USB or HLK)
    3V3       | -           | 3.3V output (for DHT11)
    GND       | -           | Ground (shared)

    Notes:
    - D4 is also connected to the onboard LED. You may see it blink when DHT11 is read.
      This is normal.
    - GPIO0, GPIO2, GPIO15 affect boot mode. Avoid using D3, D4, D8 for outputs that
      need to be in a specific state at power-on if possible.
    - Vin (5V) can supply up to about 500mA from USB. The relay module and PIR together
      draw well under 100mA so this is fine.


---


## Recommended Wire Colors (to keep things clear)

    Red    - positive power (VCC / 5V / 3.3V)
    Black  - ground (GND)
    Yellow - PIR signal wire
    Green  - DHT11 signal wire
    Blue   - Relay signal wire
    Orange or White - mains wiring (keep these clearly separate from low-voltage wires)


---


## Checking Your Wiring Before First Power-On

Before you power anything on, do these checks:

1. Check that no bare wires are touching each other.
2. Check that the relay screw terminals are tight.
3. Check that the mains section is completely separated from the low-voltage section inside your enclosure.
4. Double check that VCC goes to VCC and GND goes to GND on every component.
5. Check that D1, D2, D4 on the NodeMCU match what is set in config.h.

When you first power on, the relay should make a clicking sound when the firmware triggers it.
The PIR sensor LED (if present) should blink when it detects motion.
The Serial Monitor at 115200 baud should show the IP address within about 10 seconds.
