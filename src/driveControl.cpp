#include "main.h"
#include "MiniPID.h"

/***************************************************************************************
- POWER MULTIPLIER INCREASES SPEED OF THE DRIVE. MUILTIPLIER CAPS AT 1                 *
- RIGHT/LEFT MULTIPLIER ACCOUNTS FOR ONE SIDE BEING MORE POWERFUL                      *
- TUNE THE CONSTANTS AS NEEDED                                                         *
***************************************************************************************/

double powerMultiplier = 1;
double rightMultiplier = 1;
double leftMultiplier = 1;

int right_stick_smoothed = 0;
int left_stick_smoothed = 0;
float left_stick_prev = 0;
float right_stick_prev = 0;
int straightPathVector = 0;
int goingStraight = 0;

int deadzoneX = 10;
int deadzoneY = 5;
int t = 18; //turningCurve --> change to adjust sensitivity of turning
int d = 2; //drivingCurve --> change to adjust sensitivity of forward / backward movement

MiniPID mpid = MiniPID(0.2, 7, 1); 

//DRIVE
void setDrive(int left, int right) {
  backLeft = left;
  backRight = right;
  frontLeft = left;
  frontRight = right;
  middleLeft = left;
  middleRight = right;

}

/*
float defaultDriveCurve(float input, float scale) {
    if (scale != 0) {
        return (powf(2.718, -(scale / 10)) + powf(2.718, (fabs(input) - 127) / 10) * (1 - powf(2.718, -(scale / 10)))) *
               input;
    }
    return input;
}
*/

float defaultDriveCurve(float x, float scale) {
    float a = -2.2f;
    float b = 303.0f;
    float c = 1.8f;
    float d = -0.5f;
    float g = 3.4f;
    bool negative = false;
    float outval = 0;

    if (x < 0) {
        negative = true;
        x = fabs(x);
    }
    if (x >= 0 && x <= 42.333) {
        outval = fabs((a * powf(x, c) - g * x) / (b * powf(x, d) + 1));
    } else if (x > 42.333) {
        outval = x;
    }
    if (negative) {
        outval = outval * -1;
    }
    return outval;
}

void setDriveMotors() {

  double power = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
  double direction = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

  power *= powerMultiplier;

  //Deadzone check
  if (abs(power) <= deadzoneY)
    power = 0;
  if(abs(direction) <= deadzoneX)
    direction = 0;

  //smoothing and analog curve equation
	right_stick_smoothed = ((std::exp(-t / 12.5102293) + std::exp((std::abs(direction) - 132.55) / 69) * (1 - std::exp(-t / 10))) * direction * 0.4) + (right_stick_prev * 0.6);
	left_stick_smoothed =  ((std::exp(-d / 10) + std::exp((std::abs(power) - 100) / 10) * (1 - std::exp(-d / 10))) * power * 0.4) + (left_stick_prev * 0.6);
	right_stick_prev = right_stick_smoothed;
	left_stick_prev = left_stick_smoothed;
	//end smoothing

  //automatically correct a deviating straight path using IMU, in case wheels slip or something minor
  if (abs(direction) < deadzoneX && goingStraight == 0) {
    //if the robot is not already correcting itself for a minor deviation, set the heading normal to current IMU heading, set the flag to 1
      straightPathVector = inertial.get_rotation();
      goingStraight = 1;
    }

  //If the flag is set to 1 then do the following
  if (goingStraight == 1) {
    //if the robot is deviating, then correct by increasing the power to the side that is deviating
    if ((inertial.get_rotation()) > straightPathVector || inertial.get_rotation() < straightPathVector && abs(direction) <= 10 ) {
      //correct for left
      if (inertial.get_rotation() > straightPathVector) {
        right_stick_smoothed = right_stick_smoothed;
        }
      //correct for right
      else if (inertial.get_rotation() < straightPathVector) {
        right_stick_smoothed = right_stick_smoothed;
        }
    }
    //if the robot is back to normal, set the flag to 0
    else if (abs(direction) > 5) {
      goingStraight = 0;
    }
  }
  //end of automatic correction

  setDrive(
    defaultDriveCurve(left_stick_smoothed + right_stick_smoothed, 4),
    defaultDriveCurve(left_stick_smoothed - right_stick_smoothed, 4)
    );

  //todo: implement a functionality to automatically correct a straight path using IMU, in case wheels slip or something
}


void moveLift() {
/***************************************************************************************
- CHANGE BUTTONS IF NEED                                                               *
- CHANGE CHANGE LIFT SPEED AS NEED                                                     *
***************************************************************************************/
  bool getLiftUp = controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1);
  bool getLiftDown = controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2);
  const int liftSpeed = 115;

  if(getLiftUp) {
    lift = liftSpeed;
  }
  else if(getLiftDown) {
    lift = -liftSpeed;
  }
  else {
    lift = 0;
  }
}

void moveClamp() {
/***************************************************************************************
- CHANGE BUTTONS IF NEED                                                               *
***************************************************************************************/
  bool open = controller.get_digital(pros::E_CONTROLLER_DIGITAL_A);
  bool close = controller.get_digital(pros::E_CONTROLLER_DIGITAL_B);

  if(open) {
    piston.set_value(false);
  }
  if(close) {
    piston.set_value(true);
  }
}
/***************************************************************************************
- ADD MORE METHODS BY COPING AND PASTING THE "moveLift()" METHOD                       *
- CHANGE THE BUTTONS, LOOK AT README FOR MORE INFORMATION                              *
***************************************************************************************/


void resetMotorEncoders() {
  backLeft.tare_position();
  backRight.tare_position();
  frontRight.tare_position();
  frontLeft.tare_position();
  middleRight.tare_position();
  middleLeft.tare_position();
}

double getRightEncoder() {
  return (fabs(backRight.get_position()) + fabs(frontRight.get_position()) + fabs(middleRight.get_position())) / 3;
}

double getLeftEncoder() {
  return (fabs(frontLeft.get_position()) + fabs(backLeft.get_position()) + fabs(middleLeft.get_position())) / 3;
}

double getAvgEncoder() {
  return (fabs(backRight.get_position()) + fabs(backLeft.get_position()) +
         fabs(frontRight.get_position()) + fabs(frontLeft.get_position()) + 
         fabs(middleRight.get_position()) + + fabs(middleLeft.get_position())) / 6;
}


//TESTING
void test() {
  //USE IF NEEDED TO TEST SPECIFIC MOTORS
}
