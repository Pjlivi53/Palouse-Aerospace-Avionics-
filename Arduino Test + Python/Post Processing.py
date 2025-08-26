import serial
import time
import csv
import matplotlib.pyplot as plt
from collections import deque

#CONFIG
PORT = 'COM4'
BAUD = 115200
CSV_FILE = 'sensor_log.csv'
MAX_POINTS = 100  # max data points to show on live graph

#SETUP
arduino = serial.Serial(PORT, BAUD, timeout=1)
time.sleep(2)

# Create rolling buffers
temps, loads, pressures = deque(maxlen=MAX_POINTS), deque(maxlen=MAX_POINTS), deque(maxlen=MAX_POINTS)

# Open CSV and write headers
csvfile = open(CSV_FILE, 'w', newline='')
csvwriter = csv.writer(csvfile)
csvwriter.writerow(['Time', 'Temp_C', 'Load', 'Pressure_psi'])

#LIVE PLOT
plt.ion()
fig, (ax1, ax2, ax3) = plt.subplots(3, 1, figsize=(8, 6))
line1, = ax1.plot([], [], label='Temp (°C)')
line2, = ax2.plot([], [], label='Load')
line3, = ax3.plot([], [], label='Pressure (psi)')

for ax in (ax1, ax2, ax3):
    ax.set_xlim(0, MAX_POINTS)
    ax.grid(True)
    ax.legend(loc='upper right')

ax1.set_ylim(0, 100)   # adjust based on sensor range
ax2.set_ylim(0, 50)
ax3.set_ylim(0, 150)

def parse_line(line):
    try:
        parts = line.split('|')
        temp = float(parts[0].split(':')[1].strip())
        load = float(parts[1].split(':')[1].strip())
        pressure = float(parts[2].split(':')[1].strip())
        return temp, load, pressure
    except Exception as e:
        print("Parse error:", e, "→", line)
        return None

#MAIN LOOP
try:
    while True:
        if arduino.in_waiting:
            line = arduino.readline().decode('utf-8').strip()
            data = parse_line(line)
            if data:
                temp, load, psi = data
                timestamp = time.strftime("%H:%M:%S")
                csvwriter.writerow([timestamp, temp, load, psi])
                csvfile.flush()  # ensure real-time write

                # Add to graph buffers
                temps.append(temp)
                loads.append(load)
                pressures.append(psi)

                # Update plot
                line1.set_ydata(temps)
                line2.set_ydata(loads)
                line3.set_ydata(pressures)
                line1.set_xdata(range(len(temps)))
                line2.set_xdata(range(len(loads)))
                line3.set_xdata(range(len(pressures)))
                for ax in (ax1, ax2, ax3):
                    ax.relim()
                    ax.autoscale_view()
                plt.pause(0.01)

except KeyboardInterrupt:
    print("Stopped by user.")
finally:
    arduino.close()
    csvfile.close()


