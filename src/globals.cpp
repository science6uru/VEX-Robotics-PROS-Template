#include "main.h"


/***************************************************************************************
-CHANGE THE MOTOR PORTS BASED ON WHAT YOU HAVE                                         *
***************************************************************************************/
int pBackLeft = 3;
int pBackRight = 6;

int pFrontLeft = 1;
int pFrontRight = 4;

int pMiddleRight = 5;
int pMiddleLeft = 2;

int pLift = 9;
int pOTHER = 0;
int pOTHER1 = 0;
int pOTHER2 = 0;

int pInertial = 7;


// Motors
pros::Motor backRight(pBackRight, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor backLeft(pBackLeft, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor frontRight(pFrontRight, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor frontLeft(pFrontLeft, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor middleLeft(pMiddleLeft, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor middleRight(pMiddleRight, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);

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


