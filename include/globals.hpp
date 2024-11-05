#include "main.h"

//THESE ARE THE DRIVE MOTORS
extern pros::Motor backLeft;
extern pros::Motor backRight;
extern pros::Motor frontLeft;
extern pros::Motor frontRight;
extern pros::Motor middleLeft;
extern pros::Motor middleRight;


//THIS IS THE LIFT MOTOR
extern pros::Motor lift;

/***************************************************************************************
- DECLARE ANY EXTRA MOTORS YOU WILL NEED                                               *
***************************************************************************************/

//PISTON - COMMENT OUT OR DELETE IF NOT NEEDDED
extern pros::ADIDigitalOut piston;


extern pros::Imu inertial;

extern pros::Controller controller;

#ifdef MOTOR_CONFIG_D
extern pros::Motor* leftMotors[2];
extern pros::Motor* rightMotors[2];
#else
extern pros::Motor* leftMotors[2];
extern pros::Motor* rightMotors[2];
#endif