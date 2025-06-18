import tkinter as tk
from PIL import Image, ImageTk
from style import *
from helper import *
from matplotlib.figure import Figure
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from collections import deque

class ScrollableFrame(tk.Frame):
    def __init__(self, master=None, width= 100, height=100):
        super().__init__(master, width= width, heigh=height, bg=TEXT_BG)

        # Create a Canvas widget
        self.canvas = tk.Canvas(self, width=width,height=height, bg=TEXT_BG, relief=tk.FLAT)
        self.canvas.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)

        # Create a Scrollbar and associate it with the Canvas
        self.scrollbar = tk.Scrollbar(self, orient="vertical", bg=TEXT_BG,  command=self.on_scroll)
        self.scrollbar.pack(side=tk.RIGHT, fill=tk.Y)

        self.canvas.configure(yscrollcommand=self.scrollbar.set)

        # Bind mouse wheel event to the Canvas
        self.canvas.bind_all("<MouseWheel>", self.on_mousewheel)

        # Create a Frame inside the Canvas
        self.frame = tk.Frame(self.canvas, bg=TEXT_BG, bd=0)
        self.canvas.create_window((0, 0), window=self.frame, anchor="nw")

        # Configure bindings to update scroll region when resizing
        self.frame.bind("<Configure>", self.on_frame_configure)
        self.canvas.bind("<Configure>", self.on_canvas_configure)

    def on_mousewheel(self, event):
        self.canvas.yview_scroll(int(-1 * (event.delta / 120)), "units")

    def on_scroll(self, *args):
        self.canvas.yview(*args)

    def on_frame_configure(self, event):
        self.canvas.configure(scrollregion=self.canvas.bbox("all"))

    def on_canvas_configure(self, event):
        canvas_width = event.width
        self.canvas.itemconfig(self.frame_id, width=canvas_width)

    def update(self):
        self.frame.update_idletasks()
        self.frame_id = self.canvas.create_window(0, 0, anchor='nw', window=self.frame)
        self.on_frame_configure(None)   

class ImageFrame(tk.Label):
    def __init__(self, master, path, width=100, height=100):
        super().__init__(master)
        self.width = width
        self.height = height
        self.load_image(path)

    def load_image(self, path):
        self.img = Image.open(path)
        self.img = self.img.resize((self.width, self.height), Image.BILINEAR)
        self.tk_image = ImageTk.PhotoImage(self.img)
        self['image'] = self.tk_image



class Graph(tk.Frame):
    def __init__(self, master, width=5, height=4, maxlenth = 20, min_value = 0, max_value=10, title = "graph", ylabel = "xas"):
        super().__init__(master)

        self.maxlength = maxlenth
        self.maxvalue = max_value
        self.minvalue = min_value

        self.values = deque(maxlen= maxlenth)
        # Create a Matplotlib figure and axis
        self.lines_values = []
        self.line_names = []
        
        
        self.title = title
        self.ylabel = ylabel
        self.fig = Figure(figsize=(width, height), dpi=100, facecolor=WORKSPACE_BG)
        self.ax = self.fig.add_subplot(111)
        
        self.ax.set_facecolor(WORKSPACE_BG)
        self.ax.spines['bottom'].set_color('white')
        self.ax.spines['top'].set_color(WORKSPACE_BG)
        self.ax.spines['left'].set_color('white')
        self.ax.spines['right'].set_color(WORKSPACE_BG)
        self.ax.xaxis.label.set_color('white')
        self.ax.yaxis.label.set_color('white')
        self.ax.tick_params(axis='x', colors='white')
        self.ax.tick_params(axis='y', colors='white')

        # Create a Tkinter canvas that can be embedded in the frame
        self.canvas = FigureCanvasTkAgg(self.fig, master=self)
        self.canvas_widget = self.canvas.get_tk_widget()
        self.canvas_widget.pack(side=tk.TOP, fill=tk.BOTH, expand=1)
    
    def add_line(self, name="line"):
        values = deque(maxlen= self.maxlength)
        self.lines_values.append(values)
        self.line_names.append(name)

    def add_value(self, value, line):
        self.lines_values[line].append(value)
        #self.lines[line].set_ydata(self.lines_values[line])
        
        #self.values.append(value)
        
        self.plot_graph()
    
    def clear_values(self, line):
        self.lines_values[line].clear()
        
        self.plot_graph()
    
    def plot_graph(self):
        # Clear existing plot
        self.ax.clear()
        # Plot new data
        for values, name in zip(self.lines_values, self.line_names):
            self.ax.plot(values, label=name)
        # Set labels and title
        self.ax.set_ylabel(self.ylabel, color='white')
        self.ax.set_title(self.title, color='white')
        self.ax.set_ylim(bottom=self.minvalue, top=self.maxvalue)
        self.ax.legend(loc="upper right", facecolor=WORKSPACE_BG, edgecolor="white", labelcolor="white")
        
        # Redraw canvas
        self.canvas.draw()