# ESP32 Firmware: MQTT Temperature and Light Monitoring

This sketch reads analog sensors (NTC thermistor and LDR photoresistor) on an ESP32 and publishes the data via MQTT.

## Local Wokwi Simulation in VS Code

This directory is configured for local compilation and simulation in VS Code using the **Wokwi Simulator** extension. This avoids queues and errors from Wokwi's cloud servers ("Build Servers Busy").

### Project Files

- `sketch.ino` — ESP32 firmware source code
- `diagram.json` — hardware wiring diagram (ESP32 DevKit, NTC sensor, LDR photoresistor)
- `libraries.txt` — external library dependencies (PubSubClient)

### Step 1: Install VS Code Extensions

1. Install the **Wokwi Simulator** extension (ID: `Wokwi.wokwi-vscode`)
2. Install the **Arduino** extension by Microsoft (ID: `vsciot-vscode.vscode-arduino`)

### Step 2: Activate Wokwi License

1. Press `F1` (or `Ctrl+Shift+P`) to open the command palette
2. Type and select: **`Wokwi: Request a New License`**
3. Browser will open Wokwi website. Log in, get the license key, and confirm activation in VS Code

### Step 3: Install Libraries and Compile

#### Method A: Via Arduino Extension in VS Code (Recommended)

1. Make sure **Arduino IDE** or **Arduino CLI** is installed (required by the extension)
2. Install the **PubSubClient** library:
   - Open Arduino Library Manager (in VS Code or Arduino IDE)
   - Find and install **PubSubClient** by Nick O'Leary
3. Press the compile button (checkmark icon in top right or `Ctrl+Alt+R`)
4. Project will compile automatically, binaries (`sketch.ino.bin` and `sketch.ino.elf`) will be saved to `build/` folder

## Hardware Components

- ESP32 DevKit V1
- NTC Thermistor (temperature sensor) connected to GPIO 34
- LDR Photoresistor (light sensor) connected to GPIO 35

## Wiring

| Sensor | Pin | ESP32 Pin |
|--------|-----|-----------|
| NTC VCC | VCC | 3V3 |
| NTC GND | GND | GND.1 |
| NTC OUT | OUT | D34 |
| LDR VCC | VCC | 3V3 |
| LDR GND | GND | GND.2 |
| LDR AO | AO | D35 |

## How to Run in Wokwi

1. Open the project in Wokwi: https://wokwi.com/projects/465915722093216769
2. Click the green "Start" button to run the simulation
3. Open the Serial Monitor to see sensor readings
4. Monitor MQTT messages on HiveMQ WebSocket Client: https://www.hivemq.com/demos/websocket-client/
   - Subscribe to `vntu/chernega/#` to receive all topics

## How to Compile and Flash to Physical ESP32

### Using arduino-cli

```bash
# Install arduino-cli if not already installed
curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh

# Install ESP32 core
arduino-cli core install esp32:esp32

# Install the PubSubClient library
arduino-cli lib install "PubSubClient"

# Compile the sketch
arduino-cli compile --fqbn esp32:esp32:esp32 sketch.ino

# Flash to ESP32 (replace /dev/ttyUSB0 with your serial port)
arduino-cli upload --fqbn esp32:esp32:esp32 --port /dev/ttyUSB0 sketch.ino
```

### Using Arduino IDE

1. Install Arduino IDE and add ESP32 board support
2. Install the **PubSubClient** library via Library Manager
3. Open `sketch.ino` in Arduino IDE
4. Select board: **ESP32 Dev Module**
5. Upload the sketch

## MQTT Topics

| Topic | Data |
|-------|------|
| `vntu/chernega/temperature` | Temperature in Celsius (2 decimal places) |
| `vntu/chernega/light` | Light level in percentage (0-100%) |

Data is published every 1 second.

## Dependencies

- **WiFi.h** — ESP32 WiFi library (built-in)
- **PubSubClient.h** — MQTT client library (see `libraries.txt`)
