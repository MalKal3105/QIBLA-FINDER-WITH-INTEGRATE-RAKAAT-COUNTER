# 🕋 Qiblat Finder with Rakaat Counter (IoT Project)

This is an Internet of Things (IoT) project developed to assist Muslims in locating the Qiblat (direction of prayer) and counting Rakaat (units of prayer). It is built using the **ESP8266 WiFi Module** and programmed in **C++ (Arduino IDE)**.

> ⚠️ **Disclaimer**: The Qiblat direction functionality is currently calibrated and tested only within **Malaysia**.

## 📌 Project Summary

- **Project Title**: Qiblat Finder with Rakaat Counter
- **Technology**: IoT (Internet of Things)
- **Platform**: Arduino (ESP8266)
- **Language**: C++
- **Purpose**: To help users identify prayer direction and track rakaat count during salah.

## ⚙️ Hardware Components Used

- ESP8266 WiFi Module
- HMC5883L Magnetometer (Compass Sensor)
- OLED Display (0.96 inch, I2C)
- Push Buttons (for rakaat counting)
- Power Supply (Battery or USB)
- Breadboard / PCB for assembly
- Optional: Buzzer for rakaat feedback

## 🛠️ Software Stack

- **Arduino IDE**
- **C++**
- Uses built-in libraries and sensor drivers for the HMC5883L and OLED display.

## 💡 Features

- 📍 **Qiblat Direction**:
  - Uses magnetometer to determine compass heading.
  - Calculates and points toward Qiblat direction (fixed for Malaysia region).

- 🙏 **Rakaat Counter**:
  - Automatically counts each rakaat using a **motion sensor**.
  - Every two prostrations (sujood) are recognized and counted as **one rakaat**, following the structure of Muslim prayer.
  - The **OLED display** shows the current rakaat count in real time during prayer.

## 📷 Screenshots / Circuit Diagram

![Screenshot 2024-02-19 114921](https://github.com/user-attachments/assets/0aa5674e-d3b1-4a6b-a4ed-73011ce2642a)
![Screenshot 2024-02-19 114224](https://github.com/user-attachments/assets/3db2be3c-12e4-46e9-85d1-c626ab0c110b)
**This is my physical casing for the device using SolidWork Software**

## 🚀 How to Upload Code

1. Open the `.ino` file in Arduino IDE.
2. Select the correct board: `NodeMCU 1.0 (ESP-12E Module)` or relevant ESP8266 board.
3. Install required libraries:
   - `Wire.h`
   - `Adafruit_SSD1306`
   - `Adafruit_GFX`
   - `HMC5883L` library
4. Connect your ESP8266 to your computer via USB.
5. Click **Upload**.

## 📡 Calibration Note

- The compass (HMC5883L) must be calibrated and placed away from magnetic interference.
- The Qiblat angle is pre-calculated based on geographical location in Malaysia. You may modify the reference bearing for other countries.

## 📍 Limitations

- Designed and calibrated specifically for Malaysia.
- No GPS or location-based dynamic Qiblat calculation.
- Manual rakaat input (no automatic detection).

## 🙋‍♂️ Author

- **Name**: Muhammad Iqmal Haikal  
- **Email**: iqmalh693@gmail.com

## 📜 License

This project is developed for educational and learning purposes.

