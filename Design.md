# Design
The key elements are the LDR objects that represent a physical LDR.
It captures the sensor pin, state and trending value.

## Debug output
Define one of the following macros for useful debug output.

| Macro | Description |
| --- | --- |
| PRINT_DEBUG | Shows debug text messages to describe what is happening. |
| PLOT_ALL_VALUES | Prints the read value for each LDR. Use this with the serial plotter. |
| PLOT_DETAILS | Prints read values, moving average and threshold for the first two LDRs. Use this with the serial plotter. |

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
