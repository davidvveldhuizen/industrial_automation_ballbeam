import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from collections import deque

# Serial config
PORT = 'COM4'  # Change to your port
BAUD = 115200

ser = serial.Serial(PORT, BAUD, timeout=1)

# Settings
max_len = 100
data1 = deque([0]*max_len, maxlen=max_len)
data2 = deque([0]*max_len, maxlen=max_len)
data3 = deque([0]*max_len, maxlen=max_len)

# Set up subplots
fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(8, 6), sharex=True)
line1, = ax1.plot(data1, label="beam angle", color='r')
line3, = ax1.plot(data3, label="target angle", color='b')
line2, = ax2.plot(data2, label="ball position", color='g')

ax1.set_ylim(-30, 30)  # Adjust based on your expected value range
ax2.set_ylim(0,30)
for ax in (ax1, ax2):
    ax.legend(loc="upper left")
    ax.grid(True)

def update(frame):
    if ser.in_waiting:
        try:
            raw = ser.readline().decode().strip()
            parts = raw.split(',')
            if len(parts) == 3:
                v1 = float(parts[0])
                v2 = float(parts[1])
                v3 = float(parts[2])
                data1.append(v1)
                data2.append(v2)
                data3.append(v3)
                line1.set_ydata(data1)
                line2.set_ydata(data2)
                line3.set_ydata(data3)
                for line, data in zip((line1, line2, line3), (data1, data2, data3)):
                    line.set_xdata(range(len(data)))
        except:
            pass
    return line1, line2, line3

ani = animation.FuncAnimation(fig, update, interval=50)
plt.tight_layout()
plt.show()
