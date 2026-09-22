# ESP32 PlatformIO Project Setup

## Prerequisites

- [VS Code](https://code.visualstudio.com/) installed
- [Python 3](https://www.python.org/downloads/) installed and on your PATH (required by PlatformIO Core)

## 1. Install PlatformIO

**In VS Code:**
Install the **PlatformIO IDE** extension from the Extensions marketplace.

**In the terminal (PlatformIO Core / CLI):**
```bash
pip install -U platformio
```

If you get `pio: command not found` after installing, the binary may not be on your PATH. See the Troubleshooting section below.

## 2. Create the project

Navigate to the folder where you want the project, then run:

```bash
pio project init --board esp32dev
```

This creates the standard project structure:

```
your-project/
├── platformio.ini
├── src/
├── include/
├── lib/
└── test/
```

> **Note:** All code you want flashed to the board goes in `src/` (as `main.cpp`). PlatformIO only builds from `src/` by default — files elsewhere (e.g. a folder of old/reference code) won't be compiled unless you explicitly point PlatformIO at them.

## 3. `platformio.ini`

```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
monitor_speed = 115200
upload_speed = 921600
upload_port = COM4   ; change to your port — see below
monitor_port = COM4  ; usually same as upload_port

lib_deps = ; put libraries here
```

**Fixes/notes from the draft:**
- INI files use `;` for comments, not `//` — `//` will cause a parse error.
- `board = esp32dev` is a generic devkit ID. If you know your exact board, run `pio boards espressif32` to find the correct one — using the wrong board ID can cause upload or pin-mapping issues.
- `upload_port` / `monitor_port` are **optional** — PlatformIO auto-detects the port by default. Only set these if auto-detect picks the wrong device or you want it pinned.
- The `Adafruit SSD1306`/`GFX` libraries are only needed if you're driving an SSD1306 OLED over I2C. Delete `lib_deps` (or trim it) if your project doesn't need them.

### Finding your COM port
- **Windows:** Device Manager → Ports (COM & LPT)
- **Any OS:** `pio device list` (run in terminal) lists all connected serial devices

## 4. Build and upload

In VS Code, use the PlatformIO toolbar (bottom status bar):
- ✔ **Build**
- → **Upload**
- 🔌 **Monitor** (opens serial monitor at `monitor_speed`)

Or from the terminal:
```bash
pio run              # build
pio run -t upload    # build + upload
pio device monitor    # open serial monitor
```

## Troubleshooting

| Problem | Likely cause |
|---|---|
| `pio: command not found` | Not on PATH — use PlatformIO's built-in CLI terminal in VS Code, or add `~/.platformio/penv/bin` (macOS/Linux) or `%USERPROFILE%\.platformio\penv\Scripts` (Windows) to PATH |
| Upload fails / port busy | Another program (Arduino IDE monitor, another terminal) has the port open — close it |
| Wrong/no COM port | Run `pio device list` with the board plugged in vs. unplugged to identify it |
| I2C device not responding | Confirm wiring (SDA/SCL) and run an I2C scanner sketch to verify the device address |
