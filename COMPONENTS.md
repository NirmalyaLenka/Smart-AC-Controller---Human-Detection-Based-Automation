# Components List - Smart AC Controller

This file lists proposed components and general selection considerations. It is not a validated mains design: part ratings, example prices and suggested substitutes do not prove suitability for an air-conditioner compressor or a permanent installation.


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


### 3. Switching interface — selection must be load-specific

The active sketch drives a low-voltage relay input on D1 (GPIO5), with LOW interpreted as ON. This repository does not validate a particular relay's contacts, isolation, failure mode, thermal performance, or suitability for compressor inrush.

Do not treat a generic 10 A / 250 VAC relay as a safe minimum for an air conditioner. The actual appliance nameplate, starting/inrush current, switching category, protective devices, conductor sizing, enclosure, isolation and local standards must be evaluated together. Do not connect the relay contacts to mains based on this document. A qualified electrician must select and install the complete switching/protection assembly.

The cost and model of a suitable switching interface are intentionally left undetermined here. A higher printed current rating alone is not a guarantee of safety.

### 4. AC-to-DC power supply — proposed component only

The listed HLK-PM01 has a mains-voltage input. For development, power the NodeMCU from USB. Do not use an exposed AC/DC module as a permanent installation simply because it produces 5 V. Any mains-powered supply must be appropriately certified, enclosed, protected, installed with proper clearances and strain relief, and checked by a qualified electrician. Never handle its input side while energized.

A USB supply is the preferred option for low-voltage prototype testing.

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
    Load-rated switching interface (electrician-selected) | 1 | TBD | TBD
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


## Safety boundary

Keep the relay load terminals disconnected from mains during firmware bring-up. This BOM does not establish that the relay, fuse, power supply, conductors, or enclosure are suitable for an AC unit. Do not use generic wire-size or relay-current suggestions as an installation design. Any mains switching assembly must be selected, enclosed, protected and installed by a qualified electrician under applicable local standards.