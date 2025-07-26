# 👣 Smart Health Monitoring & Fall Detection Wearable Device

This project is part of my graduation thesis at **Hanoi University of Science and Technology**.

## 📋 Project Overview

A **wearable device** designed to monitor vital health data and detect falls in elderly users. The system continuously collects biometric and motion data and sends real-time alerts to a **mobile application** via a **cloud-based server**.

### 🔔 Key Features:
- 📡 Real-time transmission of heart rate and SpO2 data
- 📉 Fall detection using motion analysis (MPU6050)
- 📲 Mobile app alert system for caregivers or family
- ☁️ Data storage and management via Firebase + intermediate server

## 🧠 Technologies Used

| Component         | Description                                  |
|------------------|----------------------------------------------|
| `ESP32`          | Core microcontroller handling all processing |
| `MAX30102`       | Sensor for heart rate and SpO2 measurement   |
| `MPU6050`        | IMU sensor for motion and fall detection     |
| `Firebase`       | Cloud database for real-time data sync       |
| `Flutter App`    | Mobile app for displaying data & alerts      |

## 📁 Folder Structure

| Folder        | Description |
|---------------|-------------|
| `3d/`         | 3D design files for wearable enclosure (hardware housing) |
| `backup/`     | Backup of embedded code and firmware versions |
| `backup_app/` | Backup versions of the mobile app code |
| `firebase/`   | Firebase configuration and test scripts |
| `project/`    | Main codebase for ESP32 firmware and sensor integration |
| `server/`     | Intermediate server source code (Python) |

## 📦 Future Improvements

- Add GPS tracking
- Optimize battery usage
- Improve fall detection accuracy with AI models

## 📷 Screenshots




## 📬 Contact

Created by Nguyen Trung Kien
📧 trinitykien8603@gmail.com
📍 Hanoi, Vietnam
