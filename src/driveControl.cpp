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
double targetHeading = 0.0;
bool goingStraight = false;

int deadzoneX = 10;
int deadzoneY = 5;
int t = 18; //turningCurve --> change to adjust sensitivity of turning
int d = 2; //drivingCurve --> change to adjust sensitivity of forward / backward movement

MiniPID mpid = MiniPID(0.2, 7, 1); 

//DRIVE
void setDrive(int left, int right) {
  for(pros::Motor* lm: leftMotors) {
    lm->move(left);
  }
  for(pros::Motor* rm: rightMotors) {
    rm->move(right);
  }
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
  double turnSens = 0.025;
  double power = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
  double rightStick = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
  double currentHeading = inertial.get_rotation();

  power *= powerMultiplier;

  //Deadzone check
  if (abs(power) <= deadzoneY)
    power = 0;
  if(abs(rightStick) <= deadzoneX)
    rightStick = 0;

  //smoothing and analog curve equation
	right_stick_smoothed = ((std::exp(-t / 12.5102293) + std::exp((std::abs(rightStick) - 132.55) / 69) * (1 - std::exp(-t / 10))) * rightStick * 0.4) + (right_stick_prev * 0.6);
	left_stick_smoothed = ((std::exp(-d / 10) + std::exp((std::abs(power) - 100) / 10) * (1 - std::exp(-d / 10))) * power * 0.4) + (left_stick_prev * 0.6);
	right_stick_prev = right_stick_smoothed;
	left_stick_prev = left_stick_smoothed;
	//end smoothing

  targetHeading += turnSens * right_stick_smoothed;
  double dAngle = targetHeading - currentHeading;

  right_stick_smoothed = dAngle;
  
  controller.print(1,1,"curr: %s, target: %s", currentHeading, targetHeading);


/*
  //automatically correct a deviating straight path using IMU, in case wheels slip or something minor
  if (abs(direction) < deadzoneX && goingStraight == false) {
    //if the robot is not already correcting itself for a minor deviation, set the heading normal to current IMU heading, set the flag to 1
      straightPathVector = inertial.get_rotation();
      printf("%d", straightPathVector);
      goingStraight = true;
    }

  //If the flag is set to 1 then do the following
  if (goingStraight == true) {
    //if the robot is deviating, then correct by increasing the power to the side that is deviating
    if ((inertial.get_rotation()) > straightPathVector || inertial.get_rotation() < straightPathVector && abs(direction) <= 10 ) {
      //correct for left
      if (inertial.get_rotation() > straightPathVector) {
        right_stick_smoothed = right_stick_smoothed - 5;
        }
      //correct for right
      else if (inertial.get_rotation() < straightPathVector) {
        right_stick_smoothed = right_stick_smoothed + 5;
        }
    }
    //if the robot is back to normal, set the flag to 0
    else if (abs(direction) > deadzoneX + 15) {
      goingStraight = false;
    }
  }
  //end of automatic correction
  */

  setDrive(
    defaultDriveCurve(left_stick_smoothed + right_stick_smoothed, 4),
    defaultDriveCurve(left_stick_smoothed - right_stick_smoothed, 4)
    );

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
  for(pros::Motor* lm: leftMotors) {
    lm->tare_position();
  }
  for(pros::Motor* rm: rightMotors) {
    rm->tare_position();
  }
}

double getRightEncoder() {
  double sum = 0;

  for(pros::Motor* rm: rightMotors) {
    sum += (rm->get_position());
  }

  return sum / (double)(sizeof(rightMotors) / sizeof(rightMotors[0]));
}

double getLeftEncoder() {
  double sum = 0;

  for(pros::Motor* rm: leftMotors) {
    sum += (rm->get_position());
  }
  
  return sum / (double)(sizeof(leftMotors) / sizeof(leftMotors[0]));
}

double getAvgEncoder() {
  return getLeftEncoder() + getRightEncoder() / 2;
}


//TESTING
void test() {
  //USE IF NEEDED TO TEST SPECIFIC MOTORS
}
