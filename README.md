# 🌍 Real-Time Air Quality Monitoring System

A project developed to monitor air quality in real time using IoT technologies such as the ESP32, air quality sensors, and a GSM module for notifications. This system enables environmental monitoring and promotes sustainability through accessible, live data.

## 📌 Project Aim

To build a scalable and efficient real-time environmental monitoring system using ESP32 microcontrollers and provide actionable environmental data for better air quality management.

## 🎯 Objectives

-   Monitor environmental data using ESP32 and various sensors.
-   Send SMS notifications through a GSM module when unsafe levels are detected.
-   Display real-time data on a web-based React frontend.
-   Calibrate and test the entire system for deployment.

## 📍 Application Areas

-   Industrial cleanrooms (pharmaceuticals, microelectronics, etc.)
-   Environmental research
-   Smart cities
-   Public health monitoring

## ⚙️ System Architecture

### Hardware Components

-   **ESP32 Microcontroller** – Dual-core, Wi-Fi/Bluetooth-enabled chip
-   **Air Quality Sensor**
-   **Temperature and Humidity Sensor (DHT22)**
-   **Particulate Matter Sensor**
-   **GSM Module (SIM900)**
-   **Solar Panel** – Power source
-   **Battery** – Power storage
-   **tablet** – For display

### Software Stack

-   **Arduino IDE** – Code development and upload to ESP32
-   **React.js** – Frontend UI for real-time data visualization

## 🛠️ Setup & Deployment

### 1. Hardware Setup

-   Assemble ESP32 with sensors (air quality, PM, DHT22).
-   Connect GSM module, battery, and solar panel.

### 2. ESP32 Programming

-   Use Arduino IDE to program ESP32.
-   Integrate GSM for SMS notifications.
-   Set up thresholds for abnormal air quality levels.

### 3. Frontend Development

-   Built with **React.js**.
-   Fetch and visualize real-time sensor data.
-   Hosted for public access and monitoring.

### 4. Notifications

-   ESP32 detects hazardous levels.
-   GSM module sends SMS with recommendations.

### 5. Testing & Calibration

-   Ensure accuracy through calibration.
-   Perform end-to-end system testing.

## 📷 System Flow

1. Sensor Data Collection (ESP32)
2. Threshold Evaluation
3. Data sent to frontend via server
4. Alert sent via GSM if required
5. Data displayed in real-time on dashboard 

## 🎥 Project Demo Video

Watch the live demonstration of the system in action:  
👉 [Project Video on Google Drive](https://drive.google.com/file/d/1vEF0HOppr_e6Yx1RTJ4Oj-cJ2etUTJsd/view?usp=sharing)
