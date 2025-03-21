import serial
import time

# Serial port (adjust based on your setup)
SERIAL_PORT = '/dev/ttyUSB0'  # Linux/Mac, or 'COM3' for Windows
BAUD_RATE = 115200

ser = serial.Serial(SERIAL_PORT, BAUD_RATE)
start_time = time.time()
readings = 0

while True:
    if ser.in_waiting > 0:
        line = ser.readline().decode('utf-8').strip()
        print(line)
        readings += 1
    # Calculate power efficiency after 5 minutes
    if time.time() - start_time > 300:
        uptime = time.time() - start_time
        rate = readings / (uptime / 60)  # Readings per minute
        print(f"Readings per minute: {rate:.2f}")
        break

ser.close()
