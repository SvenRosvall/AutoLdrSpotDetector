# Design
The key elements are the LDR objects that represent a physical LDR.
It captures the sensor pin, state and trending value, i.e. a moving average of the read LDR value.
A threshold is set above or below this moving average.
The LDR value is compared to this calculated threshold and triggers a state change when 
the value has been above or below that threshold some time.

High Level Design
-----------------

Each LDR is connected to an analog pin and the Arduino reads
a value (0-1023) which is low for bright light and high for darkness.

The key idea is that each LDR maintains a state (covered / not covered)
and an average level.
This average level is adjusted to follow changes in the ambient light.
The algorithm for changing the average level is based on a moving exponential
average with the following formula.

```AVnext = Value * P + AVprevious * (1-P)```

A threshold is set above or below this average level.
When the LDR value crosses the threshold we treat the LDR as changing,
either as a candidate for covered or for getting uncovered.
If the LDR value stays at this side of the threshold for a given time then the
LDR is changed to a new state.

The time the LDR value has to breach the threshold is the time
it takes for the average level to be adjusted to the value where
LDR value first breached the threshold

When an LDR is considered about to be covered the other LDRs are also
checked to see if they also are getting covered. 
If the majority of the LDRs are getting covered at the same time it
is assumed that the ambient light is changing rapidly and the LDRs
are not consired to be covered anymore.

## LDR Class
Contains the following member objects:

| member | description |
|---|---|
| sensorPin | pin number that the LDR is connected to |
| ledPin | pin number that an (optional) LED is connected to. This LED makes it easy to see if the Arduino thinks the LDR is covered.|
| lastValue |
| movingAverage | captures the value trend. |
| state | one of OPEN, COVERED, COVERING, OPENING |
| threshold |
| oldThreshold |

### Member methods
#### init()
Reads the LDR value and initializes all member objects.

#### read()
Reads the LDR value and updates movingAverage and threshold.

#### updateThreshold()
Sets the threshold to a given value above or below the movingAverage.
Threshold will be above the movingAverage if OPEN and below if COVERED.

#### updateState()
Determines if the last read value breaches the threshold
in a direction that depends on the state. 
Changes state to COVERING/OPENING.

While in COVERING/OPENING state, checks
1. if the last value has suddenly changed back to a value where 
   state should be reverted.
   This is to avoid intermittent changes.
1. if mAvg has breached oldThreshold (mAvg moves slowly so this 
   is a check for stable changes)
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
