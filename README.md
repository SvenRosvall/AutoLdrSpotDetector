Auto-adjustable LDR Spot Detectors
==================================

These detectors are intended to detect presence of trains and
wagons on a model railway.
LDRs are simple and cheap detectors, but they are sensitive
to ambient light levels and usually requires adjustment potentiometers.

This sketch connects up to 6 LDR (Light Dependent Resistors) to the analog pins
and decides if each LDR is covered or not.
It considers sudden changes in light levels and also compares the LDRs to each other.

Prior work
----------
LDR ToTi with a reference LDR is described in TB 33-25. 
PCB by Alan Turner in https://merg.org.uk/merg_wiki/doku.php?id=kits:pcb_for_detector_described_in_t33_25

Geoff Gibson presents a self-setting LDR solution with a reference 
LDR in Journal 2017 No 3, page 66.

Julian Coles who presents a PIC based solution that self adjusts: 
https://merg.org.uk/forum/viewtopic.php?f=29&t=11781
David Mason mentions Arduino Nano in the same thread.

This projects brings self adjustments and reference LDRs together.
It assumes that all LDRs are never covered at the same time.
Thus it uses the uncovered LDRs as references.

Hardware
--------
The Arduino Uno has only 6 analog input pins.
The Nano has 8 analog pins but only 6 have a pull-up resistor.

The pullup resistance can be measured by setting the pin to
INPUT_PULLUP and then measure the current from this pin to ground.
On my Nanos, I get I=125uA which translates to resistance of 40kΩ.
The Arduino documentation says the pullup resistance shall be 20-50kΩ.

LDR Types
---------

| Model | Light Resistance (kOhm) | Dark Resistance (kOhm) | Arduino light level | Arduino dark level |
| ----- | ---: | ---: | ---: | ---: |
| 5506 |  2-5   |  200 |  23 | 400 |
| 5516 |  5-10  |  500 | 26 | 650 | 
| 5528 |  8-20  | 1000 | 44 |740 | 
| **5537** | 18-50  | 2000 | 55 |980 |
| 5539 | 30-90  | 5000 | 75 |990 | 
| 5549 | 45-140 |10000 | 50 | 970 | 
| 4516 |        |  | 44 | 780 | 

The Arduino values where captured on a Nano where the pull-up resistor
is 40 kOhm.
These values were coming from `analogRead(pin)`.

The LDR values vary up to 20% within a batch of the same model.

For my Nano I have chosen model 5537 as this provides the best
value range.

Timing of A/D conversion
------------------------
Expect to manage 8x inputs in <1s. 
Otherwise need to reconsider algorithm.

Found a ref saying a/d con takes 100 us.

Measured loop() iteration with 6 analogRead() calls takes 1-2 ms.

Circuit
-------

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

TODO
----
Current version is good enough for demonstrating the principles
and as a proof-of-concept. 
It will need some tweaking and fix the following problems:

- Turn on lamp with one LDR covered makes it think it is open.
  - Doesn't see that the other LDRs are also brighter now.
- 2 LDRs covered. Turn of lamp. All LEDs go on.
  - Check condition. Need to adjust count.
- LDR sometimes does not turn off LED when opened.
- Threshold value can be tricky. Say that the room gets a 
  bit darker so that only 3 LDRs trigger. Then because the
  other LDRs are not triggered, there is no consensus and the 
  triggered 3 LDRs are changed to covered.
  - Need a different algorithm where the consensus is not only
    based on the state, but if there is some change at all.
  - Use mAvg-value to deduce trend. Use this trend instead of
    LDR status when deciding consensus.
- Tuning parameters should be lifter out of AutoLdrSpotDetectors.cpp
  to the user defined sketch.
