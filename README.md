# Optimized Solar-Powered IoT Weather Station

A solar-powered weather station using ESP32 and DHT22, optimized for low power and scalability, publishing to AWS IoT Core with metrics logged to CloudWatch.

## Prerequisites
- ESP32 DevKit
- Solar panel (5V), LiPo battery, TP4056 charger
- DHT22 sensor (GPIO 4)
- Arduino IDE, `DHT`, `PubSubClient`
- AWS IoT Core (certs, endpoint)

## Setup
1. **Hardware**: Connect solar panel/battery via TP4056 to ESP32, DHT22 to GPIO 4.
2. **Firmware**: Flash `esp32_firmware/weather_station.ino` with your WiFi/AWS details.
3. **Test**: Power on, check AWS IoT Core (`weather/data`) and CloudWatch logs.
4. **Scalability**: Duplicate `client_id` (e.g., WeatherStation2) and test multiple instances.

## Optimization
- Deep sleep: 15-minute intervals.
- QoS 1 for reliable MQTT delivery.
- Metrics: Uptime, latency via CloudWatch.

## Demo
- Publishes temperature/humidity every 15 minutes.
- [Demo Screenshot](demo.png) <!-- Add after testing -->
