import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from collections import deque

# Serial Configuration
SERIAL_PORT = 'COM6'
BAUD_RATE = 9600

# Data Buffer
data_buffer = deque(maxlen=1000)

# Initialize Plot
fig, ax = plt.subplots()
line, = ax.plot([], [], lw=2)
ax.set_ylim(0, 1023)
ax.set_xlim(0, 1000)
ax.grid(True)

# Update Plot Function
def update_plot(frame):
    try:
        while ser.in_waiting:
            data = ser.readline().decode('utf-8').strip()
            if data.isdigit():
                data_buffer.append(int(data))
                line.set_data(range(len(data_buffer)), list(data_buffer))
    except Exception as e:
        print(f"Error reading data: {e}")
    return line,

# Set up Serial Connection
try:
    ser = serial.Serial(port=SERIAL_PORT, baudrate=BAUD_RATE, timeout=1)
except Exception as e:
    print(f"Could not open serial port {SERIAL_PORT}: {e}")
    exit()

# Animation with blitting enabled
ani = animation.FuncAnimation(fig, update_plot, blit=True)
plt.show()

