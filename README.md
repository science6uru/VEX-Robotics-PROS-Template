# Team 5408*'s PROS Repository
Made from a basic template PROS V5 project.

## Features

### Precision control with IMU
The IMU's internal gyroscope is used for all movement operations, in autonomous and driver control.

How it's implemented
- Wheel slip angle correction during straight driving, auton and driver control
- Precision turning and automatic error compensation using global angles during auton


TODO
- Use IMU to make turns more precise during driver control
- Use IMU to implement and tune PID in autonomous
- Unify motor encoder data with IMU data to create a more stable movement system



## Where everything is

### include > globals.hpp
    Declare any motors here.

### scr > globals.cpp
    Change the motor port variables based off of whwere they are plugged in.
    Initialize any motors that created