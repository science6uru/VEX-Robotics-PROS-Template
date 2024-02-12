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
  int direction = abs(degrees) / degrees;6
  int speed = voltage * pow(100, 0.01 * abs(abs(inertial.get_heading) - abs(direction)) / 8) - 0.3;

  inertial.reset();

  setDrive(speed * direction, speed * -direction);

  while(inertial.get_heading() > abs(degrees)) {
    pros::delay(10);
    speed = voltage * pow(100, 0.01 * abs(abs(inertial.get_heading) - abs(direction)) / 8) - 0.3;
    setDrive(speed * direction, speed * -direction);
  }
   
  setDrive(-10 * direction, 10 * direction);
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

