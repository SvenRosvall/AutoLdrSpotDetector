# Design
This design is based on Julian Coles' design where the threshold is
adjusted to cope with changes in ambient light.

High Level Design
-----------------
Every 200ms the threshold is incremented if the read LDR value is higher than the threshold
and the threshold is decremented if the LDR value is lower.

The LDR changes state if the LDR value is above/lower than the current threshold for 500ms.

## AdjustingDetectors class
In addition to fields defined in [AutoLdrSpotDetectors class](Design.md#autoldrspotdetectors-class)
the MovingAverageDetector defines the following configurable parameters:
* ```thresholdLevel``` : Determines how much above or below the moving average the threshold shall be set.

## AdjustingLDR class
In addition to fields defined in [LDR class](Design.md#ldr-class) the following fields are
defined in AdjustingLDR:
* ```timer``` : keeps track of the intervals for changing state