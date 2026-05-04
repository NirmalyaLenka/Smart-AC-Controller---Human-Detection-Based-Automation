/*
  config.h - Smart AC Controller Configuration
  -----------------------------------------------
  This is the main configuration file. Change the values here
  to customize how the system behaves. You do not need to touch
  the main .ino file unless you want to change the core logic.

  NEVER commit config_secrets.h to Git. Keep your WiFi
  credentials in that file and add it to .gitignore.
*/

#ifndef CONFIG_H
#define CONFIG_H

// -----------------------------------------------
// WIFI CREDENTIALS
// Put your actual WiFi name and password below.
// Or better: create a file called config_secrets.h
// and put them there so you do not accidentally
// share them on GitHub.
// -----------------------------------------------
#define WIFI_SSID     "YOUR_WIFI_NAME"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

// -----------------------------------------------
// PIN ASSIGNMENTS
// These match the wiring diagram. Change only if
// you have wired things to different pins.
// -----------------------------------------------
#define PIN_PIR       D2    // HC-SR501 output signal
#define PIN_RELAY     D1    // Relay module IN pin
#define PIN_DHT       D4    // DHT11 data pin
#define DHTTYPE       DHT11 // Change to DHT22 if you use that sensor

// -----------------------------------------------
// TIMING SETTINGS
// All times are in seconds unless otherwise noted.
// -----------------------------------------------

// How long after the last detected motion before the room
// is declared empty and the AC turns off.
// Default: 300 seconds (5 minutes)
#define OCCUPANCY_TIMEOUT_SEC     300

// How often the AC runs a short maintenance cycle
// when the room is empty, to keep temperature stable.
// Default: 900 seconds (15 minutes)
#define MAINTENANCE_CYCLE_SEC     900

// How long the AC runs during each maintenance cycle.
// Default: 60 seconds (1 minute)
#define MAINTENANCE_DURATION_SEC  60

// How long a manual web override lasts before the system
// automatically returns to automatic mode.
// Default: 1800 seconds (30 minutes)
#define OVERRIDE_TIMEOUT_SEC      1800

// -----------------------------------------------
// WEB SERVER
// -----------------------------------------------

// Port the web dashboard runs on.
// 80 is standard HTTP. Change to 8080 if port 80
// causes issues on your network.
#define WEB_SERVER_PORT           80

// -----------------------------------------------
// SENSOR READING INTERVAL
// How often to read temperature and humidity from
// the DHT11. The DHT11 is slow - do not go below
// 2000ms or readings will be unreliable.
// -----------------------------------------------
#define DHT_READ_INTERVAL_MS      5000

// -----------------------------------------------
// SERIAL DEBUG
// Set to 1 to print debug messages to Serial Monitor.
// Set to 0 to disable (slightly reduces memory use).
// -----------------------------------------------
#define DEBUG_SERIAL              1
#define SERIAL_BAUD               115200

// -----------------------------------------------
// RELAY POLARITY
// Most relay modules are ACTIVE LOW meaning you
// send LOW to turn the relay ON (close the circuit).
// If your relay works backwards, change this to 0.
// -----------------------------------------------
#define RELAY_ACTIVE_LOW          1

// -----------------------------------------------
// DEVICE NAME
// Shows up in the web dashboard title and Serial output.
// -----------------------------------------------
#define DEVICE_NAME               "Smart AC Controller"

// -----------------------------------------------
// OPTIONAL: Static IP (leave commented out to use DHCP)
// If you want a fixed IP address instead of whatever
// your router assigns, uncomment and fill in these lines.
// -----------------------------------------------
// #define USE_STATIC_IP
// #define STATIC_IP      "192.168.1.100"
// #define STATIC_GATEWAY "192.168.1.1"
// #define STATIC_SUBNET  "255.255.255.0"

#endif // CONFIG_H
