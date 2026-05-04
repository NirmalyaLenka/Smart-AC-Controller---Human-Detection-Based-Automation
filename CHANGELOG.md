# Changelog

All notable changes to the Smart AC Controller project are recorded here.
The format follows Keep a Changelog (keepachangelog.com).
Versions follow Semantic Versioning: MAJOR.MINOR.PATCH


---


## [1.0.0] - 2026-05-04

First public release.

### Added
- PIR-based human detection using HC-SR501 sensor
- Automatic AC on/off via 5V relay module
- 5-minute occupancy timeout before declaring room empty
- 15-minute maintenance cycle with 1-minute AC run when room is empty
- DHT11 temperature and humidity monitoring
- ESP8266 web server with live dashboard
- Manual override from web dashboard (Force On / Force Off / Return to Auto)
- 30-minute override timeout that returns to auto mode
- JSON status endpoint at /status for third-party integration
- Event log on dashboard showing last 20 events with timestamps
- Browser-based demo simulation (demo.html) requiring no hardware
- Full README with beginner-friendly setup instructions
- WIRING.md with ASCII diagrams and plain English connection guide
- COMPONENTS.md with component specs, substitutes, and sourcing info
- config.h for centralized settings without editing firmware logic
- config_secrets.h.example for safe WiFi credential management
- .gitignore preventing accidental credential commits
- MIT License with safety disclaimer


---


## Ideas for Future Versions

These are not planned yet but are worth considering.

- OTA (Over the Air) firmware updates so you do not need to physically connect USB to update
- MQTT support for integration with Home Assistant or Node-RED
- Multiple PIR sensors for larger rooms or open floor plans
- Temperature-based AC control (turn on when room exceeds a threshold, not just on motion)
- Daily and weekly schedules (always on from 10pm to 6am regardless of motion)
- Power consumption estimation based on AC runtime hours
- Telegram or WhatsApp notification when AC state changes
- EEPROM storage of settings so configuration survives power cycles without re-uploading
- Captive portal for first-time WiFi setup without editing code
- Support for IR-controlled AC units (to send power / temperature commands directly)
