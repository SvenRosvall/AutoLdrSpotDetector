# Design
This implements a simple threshold checker.
The read LDR value is checked against a fixed threshold.

The supporting classes ```ThresholdDetectors``` and ```ThresholdLDR``` don't add any member fields
and their implementing methods simply check the value of the LDR against the configured threshold.

The default state decider algorithm is the InstantStateDecider.