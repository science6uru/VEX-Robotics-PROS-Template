#include "main.h"
#include "globals.hpp"
/***************************************************************************************
-CHANGE THE MOTOR PORTS BASED ON WHAT YOU HAVE                                         *
***************************************************************************************/

#ifdef MOTOR_CONFIG_D
int pBackLeft = 13;
int pBackRight = 15;

int pFrontLeft = 19;
int pFrontRight = 14;

int pLift = 9;
int pOTHER = 0;
int pOTHER1 = 0;
int pOTHER2 = 0;

int pInertial = 17;

#else
int pBackLeft = 17;
int pBackRight = 12;

int pFrontLeft = 18;
int pFrontRight = 13;

int pArm = 10;
int pConveyor = 2;

//UNCONFIGURED
int pTopLeft = 0;
int pTopRight = 0;

int pLift = 0;
int pOTHER = 0;
int pOTHER1 = 0;
int pOTHER2 = 0;

int pInertial = 11;
#endif



// Motors
#ifdef MOTOR_CONFIG_D
pros::Motor backRight(pBackRight, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor backLeft(pBackLeft, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor frontRight(pFrontRight, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor frontLeft(pFrontLeft, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);

pros::Motor* leftMotors[2] = {&backLeft, &frontLeft};
pros::Motor* rightMotors[2] = {&backRight, &frontRight};
#else
pros::Motor backRight(pBackRight, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor frontRight(pFrontRight, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor backLeft(pBackLeft, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor frontLeft(pFrontLeft, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);

pros::Motor* leftMotors[2] = {&backLeft, &frontLeft};
pros::Motor* rightMotors[2] = {&backRight, &frontRight};
#endif

pros::Motor lift(pLift, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_ROTATIONS);

/***************************************************************************************
- INITIALIZE ANY EXTRA MOTORS YOU HAVE CREATED                                         *
***************************************************************************************/


//Piston
#define DIGITAL_SENSOR_PORT 'A'
pros::ADIDigitalOut piston('A');

// Sensors 
pros::Imu inertial(pInertial);


//Controller
pros::Controller controller(pros::E_CONTROLLER_MASTER);


