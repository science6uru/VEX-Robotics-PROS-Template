#include "main.h"

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

int deadzone = 10;
int t = 18; //turningCurve --> change to adjust sensitivity of turning
int d = 2; //drivingCurve --> change to adjust sensitivity of forward / backward movement

//DRIVE
void setDrive(int left, int right) {
  backLeft = left;
  backRight = right;
  frontLeft = left;
  frontRight = right;
}

float defaultDriveCurve(float input, float scale) {
    if (scale != 0) {
        return (powf(2.718, -(scale / 10)) + powf(2.718, (fabs(input) - 127) / 10) * (1 - powf(2.718, -(scale / 10)))) *
               input;
    }
    return input;
}

void setDriveMotors() {

  double power = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
  double direction = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

  power *= powerMultiplier;

  //Deadzone check
  if (abs(power) <= deadzone)
    setDrive(0, 0);
  if(abs(direction) <= deadzone)
    setDrive(0, 0);

  //smoothing and analog curve equation
	right_stick_smoothed = ((std::exp(-t / 12.5102293) + std::exp((std::abs(direction) - 132.55) / 69) * (1 - std::exp(-t / 10))) * direction * 0.4) + (right_stick_prev * 0.6);
	left_stick_smoothed =  ((std::exp(-d / 10) + std::exp((std::abs(power) - 100) / 10) * (1 - std::exp(-d / 10))) * power * 0.4) + (left_stick_prev * 0.6);
	right_stick_prev = right_stick_smoothed;
	left_stick_prev = left_stick_smoothed;
	//end smoothing

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
}

double getRightEncoder() {
  return (fabs(backRight.get_position()) + fabs(frontRight.get_position())) / 2;
}

double getLeftEncoder() {
  return (fabs(frontLeft.get_position()) + fabs(backLeft.get_position())) / 2;
}

double getAvgEncoder() {
  return (fabs(backRight.get_position()) + fabs(backLeft.get_position()) +
         fabs(frontRight.get_position()) + fabs(frontLeft.get_position())) / 4;
}


//TESTING
void test() {
  //USE IF NEEDED TO TEST SPECIFIC MOTORS
}
