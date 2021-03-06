#
# embedXcode
# ----------------------------------
# Embedded Computing on Xcode
#
# Copyright © Rei VILO, 2010-2016
# All rights reserved
# http://embedxcode.weebly.com
#
# Last update: Mar 28, 2017 release 7.2.7


# Libraries
# ----------------------------------
# Declare application Arduino/chipKIT/Digispark/Energia/Maple/Microduino/Teensy/Wiring
# and user's libraries used 
# Short-listing libraries speeds-up building
# Typing = 0 takes none (default for application and user)
# Otherwise, leaving empty considers all (default for local)

# List application Arduino/chipKIT/Digispark/Energia/Maple/Microduino/Teensy/Wiring libraries here
# default = 0 = none
#
# ARDUINO : AVR = Wire SoftwareSerial SPI HID EEPROM 
# ARDUINO : common = WiFi SD RobotIRremote Bridge Temboo Servo 
#	Robot_Control Stepper Robot_Motor Firmata Esplora Ethernet TFT 
#	SpacebrewYun LiquidCrystal GSM 
#
APP_LIBS_LIST = 0

# List user's libraries here 
# default = 0 = none
#
# USER = MIDI_controller Adafruit_EPD-1.0.2 DMD2 WS2812FX 
#	arduinoFFT FastLED AccelStepper Adafruit_WS2801_Library OSC DMD 
#	SdFat CapacitiveSensor OctoWS2811 Adafruit_NeoPixel 
#	ArduinoOSC Adafruit-PWM-Servo-Driver-Library 
#	Grove_Ultrasonic_Ranger-master InkShield TVout TimerOne-r11 
#
USER_LIBS_LIST = SoftwareSerial

# List local libraries here
# default = empty = all
#
LOCAL_LIBS_LIST =

# List core, application and user's libraries to be excluded
# For example, WiFi may crash on Arduino 1.0.2, Esplora on Arduino 1.0.3, Firmata on Teensy 3.0,
#   OneWire on MPIDE 0023, HardwareSerial may conflict with MarlinSerial
#
#EXCLUDE_LIBS = Firmata WiFi Esplora OneWire Robot_Control Robot_Control/utility Robot_Motor


# Warning options
# ----------------------------------
# Contrary to errors, warnings don't stop compilation but they point at possible cause of errors.
# For example, unused variables with unused-variable.
#
# If 0, no warnings
# If empty, all warnings, same as WARNING_OPTIONS = all
# WARNING_OPTIONS = all no-missing-braces no-conversion-null no-pointer-arith
# WARNING_OPTIONS = unused-variable unused-function unused-label unused-value no-conversion-null no-pointer-arith
#
WARNING_OPTIONS = 0


# Applications path folder
# ----------------------------------
# Path to the folder where the IDEs are located
# By default, = /Applications
#
#APPLICATIONS_PATH = /Applications


# Serial console for Fast target
# ----------------------------------
# For Fast target, open serial console, false or true
#
NO_SERIAL_CONSOLE = false


# Parameters
# ----------------------------------
# Xcode takes BOARD_TAG and BOARD_PORT from the .xcconfig file
# For other IDEs than Xcode,

# BOARD_TAG is required
# If not defined, error BOARD_TAG not defined
#
#BOARD_TAG  = uno

# BOARD_PORT is optional
# If not defined, BOARD_PORT = /dev/tty.usb* (default)
#
#BOARD_PORT = /dev/tty.usbmodem*

# Xcode takes SKETCHBOOK_DIR from preferences.txt
# If not defined, each IDE has its own SKETCHBOOK_DIR (default)
# To share the same SKETCHBOOK_DIR along all IDEs, define
#
#SKETCHBOOK_DIR =

# SERIAL_BAUDRATE for the serial console, 9600 by default
# Uncomment and specify another speed
#
#SERIAL_BAUDRATE = 9600

# OPTIMISATION for the optimisation and debugger levels
# Uncomment and specify another options
#
#OPTIMISATION    = -Os -g3


# Miscellaneous
# ----------------------------------
#
EMBEDXCODE_EDITION = embedXcode+ Arduino

# Manage path with space in the name
CURRENT_DIR_SPACE := $(shell pwd)
CURRENT_DIR       := $(shell echo '$(CURRENT_DIR_SPACE)' | sed 's/ /\\\ /g')

# C-compliant project name
PROJECT_NAME_AS_IDENTIFIER = Infinity
SKETCH_EXTENSION   = ino

# Include only current variant of selected platform
SELECTED_RESOURCES = false

# For distribution, keep main cpp, false or true
KEEP_MAIN_CPP = false

# For building, show details, false or true
HIDE_NUMBER = false
HIDE_COMMAND = false

# Local makefiles
#MAKEFILE_PATH     := $(CURRENT_DIR)/Makefiles
#UTILITIES_PATH    := $(CURRENT_DIR)/Utilities

# Remote makefiles
EMBEDXCODE_TOOLS   = $(HOME)/Library/embedXcode/Tools
MAKEFILE_PATH      = $(EMBEDXCODE_TOOLS)/Makefiles
UTILITIES_PATH     = $(EMBEDXCODE_TOOLS)/Utilities

# Local builds
#BUILDS_PATH_SPACE := $(CURRENT_DIR_SPACE)/Builds
#BUILDS_PATH       := $(CURRENT_DIR)/Builds

# Remote builds
BUILDS_PATH       := $(BUILT_PRODUCTS_DIR)
BUILDS_PATH_SPACE := $(BUILDS_PATH)

include $(MAKEFILE_PATH)/Step1.mk

