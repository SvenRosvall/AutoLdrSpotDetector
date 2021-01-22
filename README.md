Auto-adjustable LDR Spot Detectors
==================================

These detectors are intended to detect presence of trains and
wagons on a model railway.
LDRs are simple and cheap detectors, but they are sensitive
to ambient light levels and usually requires adjustment potentiometers.

This sketch connects up to 6 LDR (Light Dependent Resistors) to the analog pins
and decides if each LDR is covered or not.
It considers sudden changes in light levels and also compares the LDRs to each other.

The idea for this comes from
Julian Coles who presents a PIC based solution: https://merg.org.uk/forum/viewtopic.php?f=29&t=11781
David Mason mentions Arduino Nano in the same thread.

Circuit
-------
Connect each LDR between one analog input pin and ground.

Indicator LEDs are connected between a digital output and ground
with a suitable resistor in series.
Each LED indicates the status of a corresponding LDR. 
The LDRs on the analog pins correspond to the following LEDs on
digital pins:

| Analog | Digital |
| ---: | ---: |
| A0 | 10 |
| A1 | 9 |
| A2 | 8 |
| A3 | 7 |
| A4 | 6 |
| A5 | 5 |


Hardware
--------
The Arduino Uno has only 6 analog input pins.
The Nano has 8 analog pins but only 6 have a pull-up resistor.

LDR Types
---------

| Model | Light Resistance (kOhm) | Dark Resistance (kOhm) | Arduino light level | Arduino dark level |
| ----- | ---: | ---: | ---: | ---: |
| 5506 |  2-5   |  200 |  23 | 400 |
| 5516 |  5-10  |  500 | 26 | 650 | 
| 5528 |  8-20  | 1000 | 44 |740 | 
| 5537 | 18-50  | 2000 | 55 |980 |
| 5539 | 30-90  | 5000 | 75 |990 | 
| 5549 | 45-140 |10000 | 50 | 970 | 
| 4516 |        |  | 44 | 780 | 

The Arduino values where captured on a Nano where the pull-up resistor
is 40 kOhm.
These values were coming from `analogRead(pin)`.

The LDR values vary up to 20% within a batch of the same model.

For my Nano I have chosen model 5537 as this provides the best 
value range.

TODO
----
Current version is good enough for demonstrating the principles
and as a proof-of-concept. 
It will need some tweaking and fix the following problems:

- Turn on lamp with LDR covered makes it think it is open.
  - Doesn't see that the other LDRs are brighter now.
- LDR sometimes does not turn off LED when opened.
- Threshold value can be tricky. Say that the room gets a 
  bit darker so that only 3 LDRs trigger. Then because the
  other LDRs are not triggered, there is no consensus and the 
  triggered 3 LDRs are changed to covered.
  - Need a different algorithm where the consensus is not only
    based on the state, but if there is some change at all.
