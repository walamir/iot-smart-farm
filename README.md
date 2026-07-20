# 🚜 IoT Smart Farming - ESP32 Firmware

Welcome to the official ESP32 hardware repository for the **IoT Smart Farming** application ecosystem. 

This repository contains the C++ firmware required to build your own DIY smart fertigation or farming system. It reads polybag weight using an HX711 load cell and controls a water pump automatically via MQTT commands sent from our app.

---

## 🛠️ Hardware Requirements

To build this DIY Smart Farm node, you will need:
*   **1x ESP32 Development Board** (NodeMCU-32S or similar)
*   **1x HX711 Amplifier Module**
*   **1x Load Cell (Weight Sensor)** (5kg, 10kg, or 20kg depending on your polybag size)
*   **1x 5V Relay Module**
*   **1x DC Water Pump**
*   Jumper wires & power supply

---

## 🔌 Wiring Guide

Please connect your components to the ESP32 as follows. *(Note: On some ESP32 boards, the GPIO pins are labeled with a 'D', for example, D16 is GPIO 16).*

| Component | Pin Name | ESP32 Pin |
| :--- | :--- | :--- |
| **HX711** | VCC | 3.3V or 5V (Check your module) |
| **HX711** | GND | GND |
| **HX711** | DT (DOUT) | **D16 (GPIO 16)** |
| **HX711** | SCK | **D4 (GPIO 4)** |
| **Relay (Pump)** | IN / Signal | **D5 (GPIO 5)** |
| **Relay (Pump)** | VCC | 5V / VIN |
| **Relay (Pump)** | GND | GND |

---

## 💻 Software Installation

### 1. Install Dependencies
Make sure you have the Arduino IDE installed along with the ESP32 Board Manager. You will need to install the following libraries via the Arduino Library Manager:
*   **PubSubClient** by Nick O'Leary (For MQTT communication)
*   **HX711 Arduino Library** by Bogdan Necula (For Load Cell)

### 2. Configure the Firmware
1. Download the `.ino` firmware file from this repository.
2. Open it in Arduino IDE.
3. Update your Wi-Fi credentials in the code:
   ```cpp
   const char* ssid = "YOUR_WIFI_SSID";
   const char* password = "YOUR_WIFI_PASSWORD";

------------------------------------

   Upload the code to your ESP32 board.

📱 How to Get Your "Device ID" (Crucial Step!)
To connect your physical hardware to the IoT Smart Farming Android app, you need your unique Device ID.

1. After successfully uploading the code, keep your ESP32 connected to your computer.

2. Open the Serial Monitor in Arduino IDE (Set baud rate to 115200).

3. Press the EN (Reset) button on your ESP32.

4. Watch the Serial Monitor. Once connected to Wi-Fi, the ESP32 will print your unique ID generated from its MAC Address:

============================
YOUR DEVICE ID IS: ESP32-8A2B1C
Please enter this ID into the App!
============================

5. Copy this ID (e.g., ESP32-8A2B1C).

6. Open the Smart Farm App, go to Settings, and paste this Device ID.

You are now connected to the global ecosystem!
