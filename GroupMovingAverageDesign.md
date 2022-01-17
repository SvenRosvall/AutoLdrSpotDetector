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

The LDR value must stay above/below the threshold for a given
time to trigger a state change.

## LDR Class
Contains the following member objects:

| member | description |
|---|---|
| sensorPin | pin number that the LDR is connected to |
| ledPin | pin number that an (optional) LED is connected to. This LED makes it easy to see if the Arduino thinks the LDR is covered.|
| lastValue |
| movingAverage | captures the value trend. |
| movingDiffAverage | captures the change trend. |
| state | one of OPEN, COVERED, COVERING, OPENING |
| threshold |
| parent | pointer to the owning detector. Used for finding all LDRs. |

### Member methods
#### setup()
Reads the LDR value and initializes all member objects.

#### read()
Reads the LDR value and updates movingDiffAverage.

#### updateMovingAverage()
Adds the weighed diff to the movingAverage.

#### updateThreshold()
Sets the threshold to a given value above or below the movingAverage.
Threshold will be above the movingAverage if OPEN and below if COVERED.

#### updateState()
Calls updateMovingAverage() and updateThreshold().

Determines if the last read value breaches the threshold
in a direction that depends on the state. 
Changes state to COVERING/OPENING.

While in COVERING/OPENING state, checks
1. if the last value has suddenly changed back to a value where 
   state should be reverted.
   This is to avoid intermittent changes.
1. if the changeInterval has elapsed
   AND the other LDRs are not changing in the same direction.
   Then the LDR is considered to have completed its transition.

#### checkOtherLDRs()
Checks if the majority of the other LDRs are in a given state. 

## loop
The main loop runs the following operations
1. Read values for each LDR.
1. checkTransitions()
1. Update state of each LDR.
1. Debug outputs. 

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
