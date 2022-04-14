# Improving Traditional Odometry System Via Implementation Of The Accelerometer


Traditional odometry systems do not account for wheel slip.
This repository augments the existing system of the Polulu 3pi+ 32U4 robot with the accelerometer which improves positional awareness.
A weighted average combines the acceleration from both systems which calculates the position of the 3pi+.
The accelerometer has a low signal-to-noise ratio.
The noise was removed with an exponential moving average filter with a value of <img src="https://render.githubusercontent.com/render/math?math=\alpha=0.9">.
In order to detect when wheel slip occurs, the acceleration when the 3pi+ hits a wall was measured.
When a crash is detected a higher weighting is applied to the accelerometer.
By augmenting the traditional odometry system with data from the accelerometer, external forces can be accounted for, minimising the positional error and improving the performance of the 3pi+.

## Useful Files

| File Name                    | Use                                                                                                            |
| ---------------------------- | -------------------------------------------------------------------------------------------------------------- |
| ```Speed Test.ino```         | Used to measure the speed of the robot for a given PWM value                                                   |
| ```VarianceDataCollection``` | Collects data for different values of <img src="https://render.githubusercontent.com/render/math?math=\alpha"> |
| ```Drive Forward.ino```      | Run the main tests                                                                                             |
