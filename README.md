Auto-adjustable LDR Spot Detectors
==================================

These detectors are intended to detect presence of trains and
wagons on a model railway.
LDRs are simple and cheap detectors, but they are sensitive
to ambient light levels and usually requires adjustment potentiometers.

This sketch connects up to 6 LDR (Light Dependent Resistors) to the analog pins
of an arduino and decides if each LDR is covered or not.
It considers sudden changes in light levels and also compares the LDRs to each other.

Prior work
----------
LDR ToTi with a reference LDR is described in TB 33-25. 
PCB by Alan Turner in https://merg.org.uk/merg_wiki/doku.php?id=kits:pcb_for_detector_described_in_t33_25

Geoff Gibson presents a self-setting LDR solution with a reference 
LDR in Journal 2017 No 3, page 66.

Julian Coles who presented a PIC based solution that self adjusts: 
https://merg.org.uk/forum/viewtopic.php?f=29&t=11781
David Mason mentions Arduino Nano in the same thread.

This project brings self adjustments and reference LDRs together.
It assumes that all LDRs are never covered at the same time.
Thus, it uses the uncovered LDRs as references.

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

LDR Detector algorithms
-----------------------
This library contains a few different algorithms that can be
selected in the Arduino sketch.
The example sketch [DemoWitLEDs.ino](examples/DemoWithLEDs/DemoWithLEDs.ino)
is prepared for easy selection of algorithm.

The currently available algorithms are:
* [Simple Threshold](docs/ThresholdDesign.md) implements a simple threshold.
  The LDR is considered covered when the read value exceeds a set threshold value.
  This algorithm is used for testing the hardware and as a reference
  implementation to show what can be achieved with LDR detectors.
* [Adjusting Threshold](docs/AdjustingThresholdDesign.md) adjusts the threshold
  for slow changes to ambient light.
  This algorithm is based on Julian Coles' LDR detectors.
* [Moving Average](docs/MovingAverageDesign.md) implements a self adjusting 
  algorithm where a threshold is set based on an average value read from
  each LDR.
* [Group Moving Average](docs/GroupMovingAverageDesign.md) is a variation
  where a moving average is kept of the recent difference between the last value
  and the moving average value.
  The LDR is adjusted with this average diff from all LDRs. The own
  LDR diff is weighted higher.

Timing of A/D conversion
------------------------
Expect to manage 8x inputs in <1s. 
Otherwise, need to reconsider algorithm.

Found a ref saying a/d con takes 100 us.

Measured loop() iteration with 6 analogRead() calls takes 1-2 ms.

Example Circuit
-------
This project comes with an example sketch called DemoWithLEDs.
It connects LDRs to indicator LEDs to for testing and demonstrating
the LdrSpotAutoDetector.

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

## Debug output
Define one of the following macros for useful debug output.

| Macro | Description |
| --- | --- |
| PRINT_DEBUG | Shows debug text messages to describe what is happening. |
| PLOT_ALL_VALUES | Prints the read value for each LDR. Use this with the serial plotter. |
| PLOT_DETAILS | Prints read values, moving average and threshold for the first two LDRs. Use this with the serial plotter. |

TODO
----
Current version is good enough for demonstrating the principles
and as a proof-of-concept. 
It will need some tweaking and fix the following problems:

- Increase ambient light with one LDR covered makes it think it is open.
  - Doesn't see that the other LDRs are also brighter now.
- 2 LDRs covered. Turn off room light. All LEDs go on.
  - Check condition. Need to adjust count.
- LDR sometimes does not turn off LED when opened.
  Seems to happen with flickering lights such as flourescent light tubes.
  Analysis shows that the read LDR value fluctuates vividly which means that when
  an LDR is opened up some samples exceed the threshold. 
  This then cancels the current transition to open state.
- Threshold value can be tricky. Say that the room gets a 
  bit darker so that only 3 LDRs trigger. Then because the
  other LDRs are not triggered, there is no consensus, and the 
  triggered 3 LDRs are changed to covered.
  - Need a different algorithm where the consensus is not only
    based on the state, but if there is some change at all.
  - Use mAvg-value to deduce trend. Use this trend instead of
    LDR status when deciding consensus.
- Tuning parameters should be lifted out of AutoLdrSpotDetectors.cpp
  to the user defined sketch.
- Response is currently quite slow, about 1-2 seconds.
  Should react within 0.5 seconds.
  Probably need to change the idea where the reaction time is given
  by the time it takes for the moving average to reach the point where
  the LDR value first crossed the threshold.
  Change this algorithm to a fixed time or something else.
- Misses short fast trains. (single locomotive)
  This is probably due to slow reaction times as described above.
- Misses slow moving trains. 
  It is mistaken for changes in ambient light. 
  Note that the algorithm only checks the other (reference) LDRs when
  the LDR is tripped. 
  Change this to check the other LDRs when changing to COVERING.
- Add information about LDR value moving trend. This means that we
  can see if all LDRs are on the way up or down eventhough they
  haven't yet crossed their thresholds.
  Suggest using a moving average for the value changes. 
- Dave McCabe warned about coach lighting and loco headlights that
  might light up the LDRs. 
  When this light source disappears the LDR may go into covered state.
- When flickering LDR on and off rapidly (before it triggers) the 
  moving average increases slowly, and the threshold increases too.
  Then when the LDR is properly covered, the threshold is now so high
  that the LDR is not triggered.
  - Can argue that this is an artificial use case that won't happen. 

Proposed Features
---
### Use a reference LDR
Use one of the LDRs as a reference that knows for sure how
the ambient light is changing. 
This removes the need for checking the other LDRs and 
deciding a majority vote.

There is an edge case when few LDRs are used where two trains
cover one LDR each at the same time. 
This meant that there was no clear majority vote by the LDRs
and the two LDRs did not report that they changed.
A reference LDR would avoid this edge case.

### Provide more LDR inputs
Having 6 analog LDR inputs might be restrictive in some cases.
Multiplexing can be utilized to allow more LDR inputs.

Use analogue multiplexers. 
Select which input to use with a set of digital output pins.
For an 8-1 multiplexer we can now use 48 LDRs.
This requires 3 digital outputs.
Must use pullup resistors for each LDR instead of the built-in ones.
Hardware to use is one multiplexer per analog pin and 
a pullup resistor for each LDR. 

Use output pins to select a set of <= 6 lDRs.
Use external pullup resistors that are powered by digital
output pins.
The measuring point between the pull-up and the LDR connects
to the analog input via a diode.
Using 8 digital output pins for selecting the LDR to use for
each analog input gives us 48 LDRs.
This is simpler, and possibly cheaper than multiplexers.
Hardware here is one pullup resistor and a diode for each LDR.

For all solutions above the Arduino must wait for the A/D converter
to complete and produce a stable value. 
Does analogRead() do this or do we have to wait in code?

### Collaborating modules
To get a wider consensus on the ambient light level
several modules could send messages to each other.
This would allow for a bigger pool of LDRs when
deciding if a light level change is local or wide.
