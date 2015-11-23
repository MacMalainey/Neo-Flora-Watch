# Neo-Flora-Watch
LED GPS Flora watch
Uses the Adafruit Flora board, Ultimate GPS Flora Module, the 12 pixel Neopixel Ring, TMP36 temperature sensor, and a tactile switch.

Description:
This is a watch that reads the time from the GPS module and displays it using the Neopixel ring.  The time is also stored internally so that if the GPS can't find any satellties the Flora will keep track of time, so that the watch remains updated with the correct time.  Can also tell the temperature by switched modes with a tactile switch.

Libraries:
Adafruit Neopixel Library (can be found in the Arduino SDK)
Adafruit GPS (can be found in the Arduino SDK)
Software Serial (can be found in the Arduino SDK)

Display:
  Time:
	Green - Seconds
	Blue - Minutes
	Red - Hours
	12 point - White
  Temperature:
 	Orange - Postive temperature
 	Blue - Negative temperature

Circuit:
GPS:
- 3.3V -> 3.3V
- GND -> GND
- TX -> FLORA RX
- RX -> FLORA TX

NEOPIXEL RING:
- PWR -> FLORA VBATT
- GND -> GND
- IN -> FLORA D6

TACTILE SWITCH:
- END -> FLORA D12
- END -> GND

TEMPERATURE SENSOR:
- ANALOG OUT (Middle prong) -> FLORA D9
- GND -> GND
- PWR -> 3.3V
