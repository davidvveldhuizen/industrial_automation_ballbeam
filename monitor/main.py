import tkinter as tk
from tkinter import ttk
import serial
from collections import deque
from matplotlib.figure import Figure
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from widgits import Graph

# --- Serial config ---
PORT = 'COM4'
BAUD = 115200
ser = serial.Serial(PORT, BAUD, timeout=1)

# Optional: Color constant for dark theme
WORKSPACE_BG = "#1e1e1e"

# --- Main GUI ---
root = tk.Tk()
root.title("Live PID Dashboard")
root.configure(bg=WORKSPACE_BG)

# Graphs
graph1 = Graph(root, title="Beam & Target Angle", ylabel="Angle (°)", min_value=-30, max_value=30)
graph1.pack(fill=tk.BOTH, expand=True, padx=10, pady=5)
graph1.add_line("beam angle")
graph1.add_line("target angle")

graph2 = Graph(root, title="Ball Position", ylabel="Position", min_value=-15 , max_value=30)
graph2.pack(fill=tk.BOTH, expand=True, padx=10, pady=5)
graph2.add_line("ball pos avr")
graph2.add_line("ball pos raw")

# Command entry
frame = ttk.Frame(root)
frame.pack(fill=tk.X, padx=10, pady=5)

entry = ttk.Entry(frame)
entry.pack(side=tk.LEFT, fill=tk.X, expand=True)

def send_command(event=None):
    cmd = entry.get().strip()
    if cmd:
        ser.write((cmd + '\n').encode())
        entry.delete(0, tk.END)

ttk.Button(frame, text="Send", command=send_command).pack(side=tk.LEFT)
root.bind("<Return>", send_command)

# --- Periodic serial reader ---
def poll_serial():
    if ser.in_waiting:
        try:
            raw = ser.readline().decode().strip()
            parts = raw.split(',')
            #print(parts)
            if len(parts) == 4:
                v1 = float(parts[0])
                v2 = float(parts[1])
                v3 = float(parts[2])
                v4 = float(parts[3])
                
                
                graph1.add_value(v1, 0)  # Beam angle
                graph1.add_value(v2, 1)  # Target angle (overlayed)
                graph2.add_value(v3, 0)  # Ball position
                graph2.add_value(v4, 1)
            
        except Exception as e:
            print(f"Error: {e}")
    root.after(0, poll_serial)

poll_serial()
root.mainloop()
