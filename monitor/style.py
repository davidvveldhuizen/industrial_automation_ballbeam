from helper import *

WORKSPACE_BG = rgb(50,50,60)
TEXT_BG = rgb(70,70,80)

PROJECT_BUTTON_BG = rgb(30,30,60)

TASKBAR_BG = rgb(0,0,10)

FONT_COL_1 = rgb(255,255,255)
FONT_COL_2 = rgb(240,120,120)

CALENDAR_BG = rgb(10,10,20)
CALENDER_CURRENTDAY = rgb(30,30,40)
CALENDER_CURRENTDAY_WORKED = rgb(50,100,50)

WORKSPACE_TEXT = {
  "bg":WORKSPACE_BG,
  "fg":  FONT_COL_1,
  "font": ("Arial", 16)
}

WORKSPACE_BUTTON = {
  "bg":  WORKSPACE_BG,
  "fg": FONT_COL_1,
  "font" : ("Arial 14 bold")
}

WORKSPACE_ENTRY = {
  "bg":  TEXT_BG,
  "fg": FONT_COL_1,
  "insertbackground": 'white',
  "font" : ("Arial 14 bold")
}