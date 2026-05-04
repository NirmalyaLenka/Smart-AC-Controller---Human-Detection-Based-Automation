# Components List - Smart AC Controller

This file covers every component in the project: what it does, why it was chosen,
what specs to look for when buying, and what you can substitute if the exact part
is not available near you.


---


## Core Components (Required)


### 1. ESP8266 NodeMCU v3 (LoLin variant)

What it does:
    This is the brain of the whole system. It runs your code, reads the sensors,
    controls the relay, and hosts the web dashboard over Wi-Fi. The NodeMCU is
    a development board built around the ESP8266 chip. It has a USB port built in
    so you can program it directly from your laptop.

Why this one:
    The ESP8266 has Wi-Fi built into the chip itself, which means you do not need
    a separate Wi-Fi module. It is one of the cheapest Wi-Fi-capable microcontrollers
    available anywhere. The Arduino IDE supports it fully, so you write code exactly
    the same way you would for a standard Arduino.

What to look for when buying:
    - NodeMCU v2 or v3 (either works, v3 is slightly wider)
    - ESP8266 chip (not ESP32 - that is a different, more powerful chip)
    - CH340G or CP2102 USB-to-Serial chip (both work, you may need to install a driver)
    - 4MB flash memory (most NodeMCU boards have this)

Acceptable substitutes:
    - Wemos D1 Mini (smaller form factor, same ESP8266 chip, same code)
    - ESP8266 ESP-01 (much cheaper but harder to program, fewer pins, not recommended for beginners)
    - ESP32 DevKit (more powerful, pin assignments will differ, code needs minor changes)

Estimated cost: 3 to 5 USD
Search terms: "NodeMCU ESP8266 v3", "NodeMCU LoLin", "ESP8266 development board"


---


### 2. HC-SR501 PIR Motion Sensor

What it does:
    PIR stands for Passive Infrared. The sensor contains a special detector that picks up
    infrared radiation (heat). Humans give off body heat constantly. When a warm body moves
    through the sensor's detection area, the change in infrared pattern triggers the sensor
    and it sends a HIGH signal to the ESP8266.

Why this one:
    The HC-SR501 is by far the most common PIR sensor in the world. It is cheap, reliable,
    has an adjustable detection range (up to 7 meters), a wide detection angle (120 degrees),
    and two small potentiometers to tune sensitivity and output timing. It is also easy to use:
    just three wires and a single digital pin.

What to look for when buying:
    - HC-SR501 (the model number matters here, other PIR modules work differently)
    - Operating voltage 5V to 20V
    - Detection range at least 3 to 5 meters for a typical room
    - Two on-board potentiometers for sensitivity and time adjustment

Important settings on the back of the sensor:
    Sensitivity potentiometer (Sx): Turn clockwise to increase range. For a bedroom,
    halfway is usually good. If it triggers from hallway sounds or air currents, reduce it.

    Time potentiometer (Tx): Controls how long the output stays HIGH after detecting motion.
    For this project, turn it fully counterclockwise (minimum, around 3 seconds). The firmware
    handles its own occupancy timer, so you want the sensor to reset quickly.

    Jumper setting: Set to H mode (the two-pin jumper should be on the pins labeled H).
    This is "repeatable trigger" mode - the output stays HIGH as long as motion continues.
    L mode holds HIGH for a fixed time then goes LOW regardless. H mode is what we want.

Acceptable substitutes:
    - AM312 PIR module (smaller, 3.3V operation, less adjustable but works fine)
    - RCWL-0516 microwave radar sensor (works through walls, more sensitive, slightly more expensive)
    - HC-SR505 (mini version, less adjustable)

Estimated cost: 1 to 2 USD
Search terms: "HC-SR501 PIR sensor", "PIR motion sensor module"


---


### 3. 5V Single Channel Relay Module (10A / 250VAC)

What it does:
    A relay is an electrically controlled mechanical switch. The ESP8266 sends a signal
    to the relay module, which then physically connects or disconnects the mains power
    going to your AC. The relay completely isolates the mains voltage from the ESP8266,
    which is important for safety.

Why this one:
    A 5V relay module is the simplest way to control mains power with a microcontroller.
    The module version (as opposed to a bare relay) includes the transistor, flyback diode,
    and LED indicator already built in, so you just connect three wires.

What to look for when buying:
    - Relay rating: at minimum 10A / 250VAC. A 2-ton AC draws around 8 to 10 amps.
    - Coil voltage: 5V (to match the ESP8266 supply)
    - Module type (not bare relay - the module has the driver circuit built in)
    - Opto-isolated input is a bonus for extra safety between the ESP8266 and mains side

Current draw of a 2-ton AC:
    A 2-ton (24,000 BTU) air conditioner running on 230V draws approximately:
        Starting current (surge): up to 20 to 30 amps for half a second at startup
        Running current: 8 to 10 amps continuously

    This starting surge is important. A 10A relay can usually handle this surge because
    the surge is very brief. However, for extra safety, a 16A rated relay is a better choice.
    The cost difference is minimal (under 1 USD) and the peace of mind is worth it.

Acceptable substitutes:
    - 16A relay module (same wiring, better safety margin - recommended)
    - SSR-40DA Solid State Relay (no moving parts, longer life, no click sound, but more expensive around 5 USD)
    - 2-channel relay module (use only one channel, the second channel can be a spare)

Estimated cost: 1 to 2 USD
Search terms: "5V relay module 10A", "single channel relay Arduino", "5V 10A 250VAC relay module"


---


### 4. HLK-PM01 AC to DC 5V Power Module

What it does:
    This small module plugs into your mains wiring and produces 5V DC output.
    It powers the ESP8266 and all the sensors without needing a USB charger
    hanging out of a wall socket. It is what turns this project from a desk
    experiment into a proper permanent installation.

Why this one:
    The HLK-PM01 is specifically designed for embedded use. It is very small
    (about the size of a large postage stamp), reasonably efficient, and has
    built-in short circuit protection. It is widely used in similar DIY projects
    and has a good reliability record.

What to look for when buying:
    - HLK-PM01 (5V output, 600mA, 3W) or HLK-PM03 (5V, 600mA - same thing, different casing)
    - Input voltage range that includes your local mains (usually 90-264VAC, which covers both 110V and 230V)
    - At least 500mA output (the NodeMCU can draw up to 300mA under full Wi-Fi load)

Acceptable substitutes:
    - Any USB phone charger (plug it into a nearby socket, connect USB to NodeMCU - fine for testing)
    - HLK-PM03 (same specs, slightly different shape)
    - RAW-1205S (more expensive but DIN-rail mountable for panel installations)
    - AMS1117-based modules (only for 3.3V systems, will need separate 5V for relay)

Estimated cost: 2 to 4 USD
Search terms: "HLK-PM01", "Hi-Link 5V power module", "AC DC 5V 600mA module"


---


## Optional Components (Highly Recommended)


### 5. DHT11 Temperature and Humidity Sensor

What it does:
    Measures the room temperature and humidity and sends the readings to the web
    dashboard so you can see if the AC is actually doing its job. Not required for
    the AC control logic, but very useful in practice.

What to look for:
    - DHT11 breakout module (the module version has the pull-up resistor built in)
    - Operating voltage 3.3V to 5V
    - Temperature range 0 to 50 degrees Celsius (fine for indoor use)

Acceptable substitutes:
    - DHT22 (more accurate: 0.5 degree accuracy vs 2 degree for DHT11, wider range, costs 1 to 2 USD more)
    - SHT31 (more accurate still, I2C interface, costs around 3 USD)
    - DS18B20 (temperature only, no humidity, uses one-wire protocol)

Estimated cost: 1 to 2 USD
Search terms: "DHT11 module", "DHT11 breakout board"


---


## Passive Components and Hardware


### 6. Jumper Wires

For breadboard prototyping: male-to-male and male-to-female jumper wire sets.
For permanent installation: 22AWG stranded hookup wire in multiple colors.

Estimated cost: 1 to 2 USD for a set


### 7. Breadboard (for prototyping only)

Use a 400-point or 830-point breadboard to test everything before committing to
a permanent enclosure. You do not need this for the final installation.

Estimated cost: 1 to 3 USD


### 8. Project Enclosure / Junction Box

For the final installation, put all the low-voltage components (NodeMCU, PIR, DHT11,
relay module, HLK-PM01) inside a plastic project box. This protects the electronics
and keeps the mains wiring contained.

What to look for:
    - ABS plastic project box, approximately 120mm x 80mm x 40mm or larger
    - Enough room for the NodeMCU, relay module, and HLK-PM01 side by side
    - Drill holes for the PIR dome (it needs line of sight to the room) and ventilation

Estimated cost: 2 to 5 USD
Search terms: "ABS project box electronics enclosure", "plastic junction box DIY"


### 9. Screw Terminal Blocks

Makes connections more reliable than soldering for the mains wiring section.
Get 5mm pitch terminal blocks rated for at least 15A.

Estimated cost: 1 USD for a strip


### 10. 10k Ohm Resistor (only if using bare DHT11, not a module)

Acts as a pull-up resistor between the DHT11 DATA pin and VCC.
Most breakout modules already have this built in.

Estimated cost: negligible, usually comes in a resistor pack


---


## Total Bill of Materials

    Item                                  | Qty | Unit Cost | Total
    --------------------------------------|-----|-----------|-------
    NodeMCU ESP8266 v3                    |  1  | 3 - 5     | 3 - 5
    HC-SR501 PIR Sensor                   |  1  | 1 - 2     | 1 - 2
    5V Relay Module 10A / 250VAC          |  1  | 1 - 2     | 1 - 2
    HLK-PM01 5V Power Module              |  1  | 2 - 4     | 2 - 4
    DHT11 Sensor Module                   |  1  | 1 - 2     | 1 - 2
    Jumper Wires                          |  1  | 1         | 1
    Project Enclosure (120x80x40mm)       |  1  | 2 - 5     | 2 - 5
    Screw Terminal Blocks                 |  3  | 0.30      | 1
    10k Resistor (if needed)             |  1  | 0         | 0
    --------------------------------------|-----|-----------|-------
    TOTAL                                 |     |           | 12 - 22 USD


---


## Where to Buy

India (recommended sources):
    - Robocraze (robocraze.com)
    - Robu.in
    - Amazon India
    - Electronicscomp.com
    - Local electronics shops in Lal Kuan (Delhi) or Ritchie Street (Chennai)
    - Lamington Road (Mumbai)

International:
    - AliExpress (cheapest, 2 to 4 week shipping)
    - Amazon
    - Banggood

When ordering from AliExpress, order 2 of each sensor in case one is defective.
The cost difference is small and having a spare saves a lot of frustration.
