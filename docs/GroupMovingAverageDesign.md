# Design
This is a variation of the [Moving Average](MovingAverageDesign.md)
design.

High Level Design
-----------------

Similar to Moving Average, the LDR is read for a value.
The main difference is that the moving average is calculated
using a moving average of the difference of the last read value
and the moving average.

```AvgDiff = (Value-AvgDiff) * P + AvgDiff * (1-P)```

We then weigh the diff for the same LDR against all diffs for
all the other LDRs:

```Diff = R * AvgDiff + (1-R) * avg(all AvgDiffs)```

This diff is then applied to the moving average for this LDR:

```Avg = Avg + P*Diff```

A threshold is set above or below this average level.
When the LDR value crosses the threshold we treat the LDR as changing,
either as a candidate for covered or for getting uncovered.
If the LDR value stays at this side of the threshold for a given time then the
LDR is changed to a new state.

The default state decider algorithm is the TimedStateDecider.

## GroupMovingAverageDetectors Class
In addition to fields defined in [AutoLdrSpotDetectors class](Design.md#autoldrspotdetectors-class)
the MovingAverageDetector defines the following configurable parameters:
* ```thresholdLevel``` : Determines how much above or below the moving average the threshold shall be set.
* ```movingAverageP``` : The 'P' in the third [formula above](#high-level-design) that defines the moving average value.
* ```movingDiffAverageP``` : The 'P' in the first [formula above](#high-level-design) that defines the moving average of the difference.
* ```selfDiffRatio``` : The 'R' int the second [formula above](#high-level-design) that defines the difference to apply to the moving average.
* ```changeCoverInterval``` : Interval the value must be above the threshold before the LDR state is changed to COVERED.
* ```changeOpenInterval``` : Interval the value must be below the threshold before the LDR state is changed to OPEN.
* ```thresholdScaling``` : This ratio defines how much of the ```thresholdLevel``` shall be used in darker conditions. 
  This is used because the LDR values vary less in darker condition than in brighter conditions.
  Set this to 0 to skip the scaling, i.e. the ```thresholdLevel``` is used as is in both bright and dark conditions.
  Set this to 1 for full scaling.

#### checkOtherLDRs()
Checks if the majority of the other LDRs are in a given state.

### Helper functions
#### checkTransitions()
Check transition state of the LDRs and change those that are
transitioning to their target state.

This is to account for sudden changes in ambient lighting.
For example something shadows all/most of the LDRs.

#### areLdrsChanging()
Counts the number of LDRs that are in transition state and exceeding
its threshold.
Determines if all transitioning LDRs have completed their transitioning.


#### changeState()
Change any LDRs that are in a given state to a target state.

## GroupMovingAverageLDR Class
In addition to fields defined in [LDR class](Design.md#ldr-class) the following fields are
defined in GroupMovingAverageLDR:
* ```movingAverage``` : captures the value trend.
* ```movingDiffAverage``` : captures the change trend.
* ```timer``` : keeps track of the intervals for changing state

### Member methods
#### setup()
Reads the LDR value and initializes all member objects.

#### readValue()
Reads the LDR value and updates movingDiffAverage.

#### updateMovingAverage()
Adds the weighed diff to the movingAverage.

#### updateThreshold()
Sets the threshold to a given value above or below the movingAverage.
Threshold will be above the movingAverage if OPEN and below if COVERED.

#### updateState()
Calls ```updateMovingAverage()``` and ```updateThreshold()```.

Determines if the last read value breaches the threshold
in a direction that depends on the state. 
Changes state to COVERING/OPENING.

While in COVERING/OPENING state, checks
1. if the last value has suddenly changed back to a value where 
   state should be reverted.
   This is to avoid intermittent changes.
1. if the changeCoverInterval/changeOpenInterval has elapsed
   AND the other LDRs are not changing in the same direction.
   Then the LDR is considered to have completed its transition.
