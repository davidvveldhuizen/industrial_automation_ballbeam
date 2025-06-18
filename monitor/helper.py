import tkinter as tk

def rgb(r,g,b):
    """translates an rgb tuple of int to a tkinter friendly color code
    """
    
    rgb = (r,g,b)
    return "#%02x%02x%02x" % rgb

def intarray(array):
  new_array = []
  for i in array:
    new_array.append(int(i))
  return new_array

def switch_Frame(frames, frame_index):
  for i,frame in enumerate(frames):
    if i == frame_index:
      frame.tkraise()
      frame.pack(fill=tk.BOTH, expand=True)
    else:
      frame.forget()

def clear_frame(frame):
  for widgit in frame.winfo_children():
    widgit.destroy()
  frame.pack_forget()

def time_diff(start, stop):
  start_min = start[0]*60 + start[1]
  stop_min = stop[0]*60 + stop[1]
  
  total_minutes = stop_min - start_min
  if total_minutes < 0:
    raise ValueError("stop time can not be lower then start time")
  
  hours = int(total_minutes // 60)
  minutes = int(total_minutes % 60)
  
  return hours, minutes