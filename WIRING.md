# Smart AC Controller — Low-Voltage Wiring Reference

## Safety boundary

This repository is a prototype, not a certified air-conditioner or mains-switching design. The previous mains-side diagram and step-by-step live-wire instructions have been removed. Do not connect the relay contacts to an air conditioner or other mains load based on this repository.

For bring-up, power the NodeMCU from USB and keep the relay's load terminals disconnected from mains. A relay's printed current rating does not prove suitability for compressor inrush, continuous load current, isolation, fault conditions, or the applicable electrical code. Any eventual mains switching interface must be selected, enclosed, protected, inspected, and installed by a qualified electrician for the exact appliance and local standards. Never work on energized mains wiring.

## Components used by the current sketch

- NodeMCU ESP8266-compatible board
- HC-SR501 PIR motion sensor
- DHT11 temperature/humidity sensor
- Low-voltage relay module used only as a control output during initial testing
- USB data cable for development

The repository does not establish that the relay module or any mains power supply is suitable for a permanent installation.

## Controller-side pin table

These pins are defined in smart_ac_controller.ino. Do not change them in the wiring unless the firmware is changed separately and reviewed.

| NodeMCU pin | ESP8266 GPIO | Connect to | Notes |
|---|---:|---|---|
| D2 | GPIO4 | HC-SR501 OUT | Firmware treats HIGH as motion detected. |
| D4 | GPIO2 | DHT11 DATA | DHT11 is the active type in the current sketch. |
| D1 | GPIO5 | Relay module IN | Firmware drives LOW for relay ON and HIGH for OFF. |
| VIN / 5 V | — | PIR supply, if supported by the exact module | Confirm the module's input range before powering it. |
| 3.3 V | — | DHT11 VCC, if supported by the exact sensor board | Follow the sensor board's own rating. |
| GND | — | Sensor and control-side grounds | Use a common low-voltage reference as required by the modules. |

## Low-voltage bring-up checklist

1. Leave all relay load/contact terminals disconnected from mains.
2. Inspect the circuit for shorts before connecting USB.
3. Upload the sketch to the matching ESP8266 board and open Serial Monitor at 115200 baud.
4. Confirm Wi-Fi connection and record the IP address printed at startup.
5. Test PIR and DHT11 readings while the relay output remains disconnected from mains.
6. If testing relay logic, use only an isolated low-voltage test load that is appropriate for the module and your experience.

## Behavior and limitations relevant to wiring

- Occupancy timeout: 300 seconds.
- Empty-room maintenance cycle: after 900 seconds, the firmware turns the relay on for 60 seconds.
- Manual override timeout: 1800 seconds.
- The relay polarity is hardcoded in the sketch as active LOW; the setting in config (1).h is not used by the active firmware.
- The HTTP control routes have no authentication. Keep the controller on a trusted LAN and do not expose its port to the public internet.
- This document does not validate a mains-side circuit, relay rating, fuse, power supply, enclosure, or installation.

See README.md for software setup and current configuration limitations.