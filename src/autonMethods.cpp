#include "main.h"

void translate(int units, int voltage) {
   int direction = abs(units) / units;

   resetMotorEncoders();

   while(getAvgEncoder() < abs(units)) {
       setDrive(voltage * direction, voltage * direction);
       pros::delay(10);
   }

   setDrive(-10 * direction, -10 * direction);
   pros::delay(10);

   setDrive(0, 0);
}

void translateInertial(int units, int voltage) {
  int direction = abs(units) / units;

  inertial.reset();

  setDrive(voltage * direction, voltage * direction);
  while(inertial.get_heading() > abs(units)) {
    pros::delay(10);
  }
   
  setDrive(-10 * direction, -10 * direction);
  pros::delay(10);

  setDrive(0, 0);
}

void turnInertial(int degrees, int voltage) {
  //Determine if turn should be left or right
  int direction = abs(degrees) / degrees;
  //Calc initial speed from distance to target and voltage cap
  int speed = voltage * pow(100, 0.01 * abs(abs(inertial.get_heading) - abs(direction)) / 8) - 0.3;

  inertial.reset();

  setDrive(speed * direction, speed * -direction);

  //Loop below until it reaches the target degree
  while(inertial.get_heading() > abs(degrees)) {
    pros::delay(10);

    //Lower speed as it approaches target degree. not PID, but has the right effect
    speed = voltage * pow(100, 0.01 * abs(abs(inertial.get_heading) - abs(direction)) / 8) - 0.3;
    setDrive(speed * direction, speed * -direction);
  }
   
  //setDrive(-10 * direction, 10 * direction);
  pros::delay(10);

  setDrive(0, 0);
}

void turn(int units, int voltage) {
  int direction = abs(units) / units;

  resetMotorEncoders();

  while(getAvgEncoder() < abs(units)) {
      setDrive(voltage * direction, voltage * -direction);
      pros::delay(10);
  }

  setDrive(-10 * direction, -10 * direction);
  pros::delay(10);

  setDrive(0, 0);
}

