# ESP32-Tutorial

#### Description

This project shows off the capacity of the ESP-32. The ESP-32 is a Microcontroller board you can use for Electronics that also has Wifi and Bluetooth. The current code in this repo can connect to Wifi and allows one to control a LED via a REST API.

#### Example API

``` bash
HTTP server started
API Ready! Use:
  GET  http://10.0.0.213/
  GET  http://10.0.0.213/led/on
  GET  http://10.0.0.213/led/off
  POST http://10.0.0.213/led  + JSON: {"state":"on"}
```

#### Serial via Terminal

``` bash
# List Devices Linux
ls /dev/tty*

# List devices MacOS
ls /dev/tty.*

# Replace /dev/tty* with your device
screen /dev/tty.usbserial-0166A67D 115200

```


#### AI Convos and Resources

**2025-11-06**

- Write a Wifi Connection ESP32 Program
  - https://grok.com/share/bGVnYWN5_2fc130eb-3bc8-4b2a-9cdb-b67f79afb717
- Connect to Serial via Terminal
  - https://grok.com/share/bGVnYWN5_5f8aa069-7c93-40df-a1d9-9c312a2893ec
- How to set BAUD rate for PlatformIO Serial Monitor
  - [how to configure serial port monitor in platformio | HowTo.IM - Expert Answers & Tutorials | HowTo.IM](https://howto.im/q/how-to-configure-serial-port-monitor-in-platformio)
- Generate the LED controlled via REST API SCript
  - https://grok.com/share/bGVnYWN5_e0f46b9d-c887-488b-9fcc-3ae0aee1e6d9