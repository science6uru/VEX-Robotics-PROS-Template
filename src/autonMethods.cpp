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
  //Determine if turn should be left or right
  int direction = abs(units) / units;
  inertial.reset();
  resetMotorEncoders();

  int straightPathNormal = inertial.get_heading();
  

  //TODO Implement PID system using IMU
  int voltage1 = voltage;
  int voltage2 = voltage;

  setDrive(voltage * direction, voltage * direction);
  
  //Loop below until it reaches the target distance (Using encoder values for now)
  while(getAvgEncoder() < abs(units)) {
    pros::delay(10);

    if (inertial.get_heading() > straightPathNormal || inertial.get_heading() < straightPathNormal) {
      if (inertial.get_heading() > straightPathNormal) {
        voltage1 = voltage - 2;
      }
      else if (inertial.get_heading() < straightPathNormal) {
        voltage2 = voltage - 2;
      }
      else {
        voltage1 = voltage;
        voltage2 = voltage;
      }
    }

    //Lower drive speed as it approaches target distance
    if (getAvgEncoder() > abs(units) * 0.95) {
      setDrive(voltage1 * 0.1 * direction, voltage2 * 0.1 * direction);
    }
    else if (getAvgEncoder() > abs(units) * 0.9) {
      setDrive(voltage1 * 0.2 * direction, voltage2 * 0.2 * direction);
    }
    else if (getAvgEncoder() > abs(units) * 0.7) {
      setDrive(voltage1 * 0.4 * direction, voltage2 * 0.4 * direction);
    }
    else if (getAvgEncoder() > abs(units) * 0.5) {
      setDrive(voltage1 * 0.8 * direction, voltage2 * 0.8 * direction);
    }
    else {
      setDrive(voltage1 * direction, voltage2 * direction);
    }

  }
   
  setDrive(-10 * direction, -10 * direction);
  pros::delay(10);

  setDrive(0, 0);
}

void turnInertial(int degrees, int voltage) {
  //Determine if turn should be left or right
  int direction = abs(degrees) / degrees;
  //Calc initial speed from distance to target and voltage cap
  int speed = voltage * pow(100, 0.01 * abs(abs(inertial.get_heading()) - abs(direction)) / 8) - 0.3;

  inertial.reset();

  setDrive(speed * direction, speed * -direction);

  //Loop below until it reaches the target degree
  while(inertial.get_heading() > abs(degrees)) {
    pros::delay(10);

    //Lower speed as it approaches target degree. not PID, but has the right effect
    speed = voltage * pow(100, 0.01 * abs(abs(inertial.get_heading()) - abs(direction)) / 8) - 0.3;
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

