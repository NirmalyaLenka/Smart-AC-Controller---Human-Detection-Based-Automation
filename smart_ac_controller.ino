/*
  Smart AC Controller - Human Detection Based Automation
  -------------------------------------------------------
  Hardware:
    - ESP8266 NodeMCU v3
    - HC-SR501 PIR Motion Sensor on D2 (GPIO4)
    - DHT11 Temperature Sensor on D4 (GPIO2)
    - 5V Relay Module on D1 (GPIO5)

  How it works:
    - If someone is in the room, AC stays on continuously
    - If no motion for 5 minutes, room is considered empty, AC turns off
    - While empty, every 15 minutes the AC runs for 1 minute to maintain temperature
    - A web server on the ESP8266 lets you see status and override from your phone/browser
*/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>
#include <ArduinoJson.h>

// -----------------------------------------------
// CONFIGURE THESE BEFORE UPLOADING
// -----------------------------------------------
const char* ssid     = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

// Timing settings (all in seconds unless noted)
const unsigned long occupancyTimeoutSec   = 300;   // 5 minutes - how long after last motion before room is "empty"
const unsigned long maintenanceCycleSec   = 900;   // 15 minutes - how often AC runs when room is empty
const unsigned long maintenanceDurationSec= 60;    // 1 minute  - how long AC runs during maintenance cycle

// Manual override timeout - if you force AC on/off from web, it overrides for this long
const unsigned long overrideTimeoutSec    = 1800;  // 30 minutes
// -----------------------------------------------

// Pin definitions
#define PIN_PIR     D2   // GPIO4 - PIR sensor output
#define PIN_RELAY   D1   // GPIO5 - Relay control (LOW = relay ON for most relay modules)
#define PIN_DHT     D4   // GPIO2 - DHT11 data pin
#define DHTTYPE     DHT11

DHT dht(PIN_DHT, DHTTYPE);
ESP8266WebServer server(80);

// State variables
bool roomOccupied       = false;
bool acOn               = false;
bool manualOverride     = false;
bool overrideState      = false;

unsigned long lastMotionTime        = 0;  // millis() when we last saw motion
unsigned long maintenanceStartTime  = 0;  // millis() when last maintenance cycle started
unsigned long maintenanceTimer      = 0;  // millis() when last maintenance cycle ended (or room became empty)
unsigned long overrideStartTime     = 0;

float currentTemp     = 0;
float currentHumidity = 0;
unsigned long lastDHTRead = 0;

// Log buffer (stores last 20 events)
String eventLog[20];
int logIndex = 0;

// -----------------------------------------------
// Helpers
// -----------------------------------------------

void setRelay(bool on) {
  // Most relay modules are active LOW: LOW = relay closed = AC on
  digitalWrite(PIN_RELAY, on ? LOW : HIGH);
  acOn = on;
}

void logEvent(String msg) {
  // Store event with timestamp (seconds since boot)
  unsigned long t = millis() / 1000;
  unsigned long h = t / 3600;
  unsigned long m = (t % 3600) / 60;
  unsigned long s = t % 60;

  char ts[20];
  snprintf(ts, sizeof(ts), "%02lu:%02lu:%02lu", h, m, s);

  eventLog[logIndex % 20] = String(ts) + " - " + msg;
  logIndex++;

  Serial.println(eventLog[(logIndex - 1) % 20]);
}

// -----------------------------------------------
// Web server handlers
// -----------------------------------------------

// Serve the main dashboard HTML page
void handleRoot() {
  // Read the dashboard HTML - we embed it here as a string for simplicity
  // In a real setup you would store this in SPIFFS/LittleFS
  String html = R"rawhtml(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Smart AC Controller</title>
<style>
  * { box-sizing: border-box; margin: 0; padding: 0; }
  body {
    font-family: 'Courier New', monospace;
    background: #0d1117;
    color: #c9d1d9;
    min-height: 100vh;
    padding: 20px;
  }
  h1 { color: #58a6ff; font-size: 1.4em; margin-bottom: 6px; }
  .subtitle { color: #8b949e; font-size: 0.8em; margin-bottom: 24px; }
  .card {
    background: #161b22;
    border: 1px solid #30363d;
    border-radius: 8px;
    padding: 16px;
    margin-bottom: 16px;
  }
  .card h2 { font-size: 0.75em; color: #8b949e; text-transform: uppercase; letter-spacing: 1px; margin-bottom: 12px; }
  .status-row { display: flex; gap: 12px; flex-wrap: wrap; }
  .badge {
    padding: 6px 14px;
    border-radius: 20px;
    font-size: 0.85em;
    font-weight: bold;
  }
  .badge.green  { background: #0d4429; color: #3fb950; border: 1px solid #238636; }
  .badge.red    { background: #2d0f0f; color: #f85149; border: 1px solid #6e2222; }
  .badge.yellow { background: #3d2f00; color: #e3b341; border: 1px solid #6e4500; }
  .badge.gray   { background: #21262d; color: #8b949e; border: 1px solid #30363d; }
  .stat { margin-top: 10px; font-size: 0.9em; }
  .stat span { color: #58a6ff; }
  .btn-row { display: flex; gap: 10px; flex-wrap: wrap; margin-top: 10px; }
  button {
    padding: 9px 18px;
    border: none;
    border-radius: 6px;
    font-family: inherit;
    font-size: 0.85em;
    cursor: pointer;
    font-weight: bold;
  }
  .btn-on  { background: #238636; color: #fff; }
  .btn-off { background: #b62324; color: #fff; }
  .btn-auto{ background: #1f6feb; color: #fff; }
  .log { font-size: 0.75em; color: #8b949e; max-height: 180px; overflow-y: auto; }
  .log div { padding: 3px 0; border-bottom: 1px solid #21262d; }
  .progress-bar-wrap { background: #21262d; border-radius: 4px; height: 8px; margin-top: 8px; }
  .progress-bar { height: 8px; border-radius: 4px; background: #1f6feb; transition: width 0.5s; }
</style>
</head>
<body>
<h1>Smart AC Controller</h1>
<div class="subtitle">ESP8266 + HC-SR501 PIR + DHT11</div>

<div class="card">
  <h2>Room Status</h2>
  <div class="status-row">
    <div id="occupancy" class="badge gray">Loading...</div>
    <div id="acstate"   class="badge gray">Loading...</div>
    <div id="override"  class="badge gray" style="display:none">Manual Override</div>
  </div>
  <div class="stat" id="timerInfo">Loading...</div>
  <div class="progress-bar-wrap" id="progressWrap" style="display:none">
    <div class="progress-bar" id="progressBar" style="width:0%"></div>
  </div>
</div>

<div class="card">
  <h2>Environment</h2>
  <div class="stat">Temperature: <span id="temp">--</span> C</div>
  <div class="stat">Humidity: <span id="hum">--</span> %</div>
</div>

<div class="card">
  <h2>Manual Control</h2>
  <div class="btn-row">
    <button class="btn-on"  onclick="cmd('on')">Force AC On</button>
    <button class="btn-off" onclick="cmd('off')">Force AC Off</button>
    <button class="btn-auto" onclick="cmd('auto')">Return to Auto</button>
  </div>
  <div class="stat" style="margin-top:8px;color:#8b949e;font-size:0.78em">
    Manual override lasts 30 minutes then returns to auto.
  </div>
</div>

<div class="card">
  <h2>Event Log</h2>
  <div class="log" id="log">Loading...</div>
</div>

<script>
function cmd(action) {
  fetch('/control?action=' + action)
    .then(() => refresh())
    .catch(e => console.error(e));
}

function refresh() {
  fetch('/status')
    .then(r => r.json())
    .then(d => {
      document.getElementById('temp').textContent = d.temperature.toFixed(1);
      document.getElementById('hum').textContent  = d.humidity.toFixed(0);

      const occ = document.getElementById('occupancy');
      occ.textContent = d.occupied ? 'Room Occupied' : 'Room Empty';
      occ.className   = 'badge ' + (d.occupied ? 'green' : 'gray');

      const ac = document.getElementById('acstate');
      ac.textContent = d.acOn ? 'AC ON' : 'AC OFF';
      ac.className   = 'badge ' + (d.acOn ? 'green' : 'red');

      const ov = document.getElementById('override');
      ov.style.display = d.manualOverride ? 'block' : 'none';

      const info = document.getElementById('timerInfo');
      const wrap = document.getElementById('progressWrap');
      const bar  = document.getElementById('progressBar');

      if (d.occupied) {
        info.innerHTML = 'Last motion <span>' + d.secondsSinceMotion + 's</span> ago. Auto-off in <span>' + Math.max(0, d.occupancyTimeout - d.secondsSinceMotion) + 's</span>';
        wrap.style.display = 'block';
        bar.style.width = Math.min(100, (d.secondsSinceMotion / d.occupancyTimeout) * 100) + '%';
        bar.style.background = '#3fb950';
      } else {
        const nextIn = Math.max(0, d.maintenanceCycle - d.secondsIntoMaintenanceCycle);
        info.innerHTML = d.inMaintenance
          ? 'Maintenance cycle active. AC off in <span>' + Math.max(0, d.maintenanceDuration - d.secondsInMaintenance) + 's</span>'
          : 'Next maintenance cycle in <span>' + nextIn + 's</span>';
        wrap.style.display = 'block';
        bar.style.background = '#e3b341';
        bar.style.width = d.inMaintenance
          ? (d.secondsInMaintenance / d.maintenanceDuration * 100) + '%'
          : (d.secondsIntoMaintenanceCycle / d.maintenanceCycle * 100) + '%';
      }

      const log = document.getElementById('log');
      log.innerHTML = d.log.map(l => '<div>' + l + '</div>').join('');
    })
    .catch(e => { console.error(e); });
}

refresh();
setInterval(refresh, 3000);
</script>
</body>
</html>
)rawhtml";

  server.send(200, "text/html", html);
}

// Return JSON status
void handleStatus() {
  unsigned long nowMs = millis();
  unsigned long nowSec = nowMs / 1000;

  // Figure out maintenance timing
  unsigned long secIntoMaintenanceCycle = (nowMs - maintenanceTimer) / 1000;
  bool inMaintenance = false;
  unsigned long secInMaintenance = 0;

  if (!roomOccupied && maintenanceStartTime > 0) {
    secInMaintenance = (nowMs - maintenanceStartTime) / 1000;
    inMaintenance = (secInMaintenance < maintenanceDurationSec);
  }

  StaticJsonDocument<800> doc;
  doc["occupied"]                   = roomOccupied;
  doc["acOn"]                       = acOn;
  doc["manualOverride"]             = manualOverride;
  doc["temperature"]                = currentTemp;
  doc["humidity"]                   = currentHumidity;
  doc["secondsSinceMotion"]         = (nowMs - lastMotionTime) / 1000;
  doc["occupancyTimeout"]           = occupancyTimeoutSec;
  doc["maintenanceCycle"]           = maintenanceCycleSec;
  doc["maintenanceDuration"]        = maintenanceDurationSec;
  doc["inMaintenance"]              = inMaintenance;
  doc["secondsIntoMaintenanceCycle"]= secIntoMaintenanceCycle;
  doc["secondsInMaintenance"]       = secInMaintenance;

  JsonArray logArr = doc.createNestedArray("log");
  // Return log in reverse order (newest first)
  int total = min(logIndex, 20);
  for (int i = total - 1; i >= 0; i--) {
    logArr.add(eventLog[(logIndex - 1 - i + 20) % 20]);
  }

  String out;
  serializeJson(doc, out);
  server.send(200, "application/json", out);
}

// Handle manual control commands
void handleControl() {
  if (!server.hasArg("action")) {
    server.send(400, "text/plain", "Missing action");
    return;
  }

  String action = server.arg("action");

  if (action == "on") {
    manualOverride  = true;
    overrideState   = true;
    overrideStartTime = millis();
    setRelay(true);
    logEvent("Manual override: AC forced ON");
  } else if (action == "off") {
    manualOverride  = true;
    overrideState   = false;
    overrideStartTime = millis();
    setRelay(false);
    logEvent("Manual override: AC forced OFF");
  } else if (action == "auto") {
    manualOverride = false;
    logEvent("Returned to automatic mode");
  }

  server.send(200, "text/plain", "OK");
}

// -----------------------------------------------
// Setup
// -----------------------------------------------
void setup() {
  Serial.begin(115200);
  delay(100);

  pinMode(PIN_PIR,   INPUT);
  pinMode(PIN_RELAY, OUTPUT);
  digitalWrite(PIN_RELAY, HIGH); // Start with relay off (active LOW module)

  dht.begin();

  Serial.println("\nSmart AC Controller starting...");

  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected to WiFi");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    logEvent("System started. WiFi connected. IP: " + WiFi.localIP().toString());
  } else {
    Serial.println("\nWiFi failed - running in offline mode");
    logEvent("System started. WiFi not connected.");
  }

  // Set up web server routes
  server.on("/",        handleRoot);
  server.on("/status",  handleStatus);
  server.on("/control", handleControl);
  server.begin();
  Serial.println("Web server started");

  // Init timers
  lastMotionTime   = millis();
  maintenanceTimer = millis();

  logEvent("Waiting for motion...");
}

// -----------------------------------------------
// Main loop
// -----------------------------------------------
void loop() {
  server.handleClient();

  unsigned long nowMs = millis();

  // Read temperature / humidity every 5 seconds
  if (nowMs - lastDHTRead > 5000) {
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    if (!isnan(t)) currentTemp     = t;
    if (!isnan(h)) currentHumidity = h;
    lastDHTRead = nowMs;
  }

  // Check for manual override expiry
  if (manualOverride && (nowMs - overrideStartTime) / 1000 > overrideTimeoutSec) {
    manualOverride = false;
    logEvent("Manual override expired. Returning to auto.");
  }

  // If manual override is active, skip all automation logic
  if (manualOverride) return;

  // Read PIR sensor
  bool motionDetected = digitalRead(PIN_PIR) == HIGH;

  if (motionDetected) {
    lastMotionTime = nowMs;

    if (!roomOccupied) {
      roomOccupied = true;
      maintenanceTimer = 0; // reset maintenance timer reference
      maintenanceStartTime = 0;
      setRelay(true);
      logEvent("Motion detected. Room occupied. AC turned ON.");
    }
  }

  // Check if we should consider the room empty
  if (roomOccupied) {
    unsigned long secSinceMotion = (nowMs - lastMotionTime) / 1000;

    if (secSinceMotion >= occupancyTimeoutSec) {
      roomOccupied     = false;
      maintenanceTimer = nowMs; // start maintenance cycle clock
      maintenanceStartTime = 0;
      setRelay(false);
      logEvent("No motion for " + String(occupancyTimeoutSec) + "s. Room empty. AC turned OFF.");
    }
  }

  // Maintenance cycle when room is empty
  if (!roomOccupied) {
    unsigned long secInCycle = (nowMs - maintenanceTimer) / 1000;

    // Time to start a maintenance run
    if (maintenanceStartTime == 0 && secInCycle >= maintenanceCycleSec) {
      maintenanceStartTime = nowMs;
      setRelay(true);
      logEvent("Maintenance cycle started. AC will run for " + String(maintenanceDurationSec) + "s.");
    }

    // Time to end a maintenance run
    if (maintenanceStartTime > 0) {
      unsigned long secInMaintenance = (nowMs - maintenanceStartTime) / 1000;
      if (secInMaintenance >= maintenanceDurationSec) {
        maintenanceStartTime = 0;
        maintenanceTimer     = nowMs; // reset cycle clock
        setRelay(false);
        logEvent("Maintenance cycle complete. AC OFF. Next cycle in " + String(maintenanceCycleSec) + "s.");
      }
    }
  }

  delay(100); // small delay to avoid hammering the loop
}
