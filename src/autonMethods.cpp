#include "main.h"
#include <cmath>
#include "MiniPID.h"

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
  double destination = units;
  int direction = abs(units) / units;

  inertial.reset();
  resetMotorEncoders();
  int distance = getAvgEncoder();
  int straightPathNormal = inertial.get_heading();
  //PID setup
  //https://pidexplained.com/pid-controller-explained/
  //https://grauonline.de/alexwww/ardumower/pid/pid.html
  MiniPID mpid = MiniPID(0.2, 7, 1); 
	mpid.setOutputLimits(-128,127);
	mpid.setOutputRampRate(10);
  
  int voltage1 = voltage;
  int voltage2 = voltage;
  setDrive(voltage * direction, voltage * direction);
  
  //Loop below until it reaches the target distance
  while(destination - distance > 0.1) {
    pros::delay(10);
    pros::c::imu_accel_s_t accel = inertial.get_accel();
    int acceleration = accel.y; //change me depending on how the IMU is mounted
    distance = getAvgEncoder();
    //automatically correct a deviating straight path using IMU, in case wheels slip or something minor
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
    //Use PID to lower speed as it approaches target distance and optimize for speed and accuracy
    double output = mpid.getOutput(distance, destination);
    //double velOutput = mpid.getOutput(acceleration, output);
    setDrive((voltage1 * output) * direction, (voltage2 * output) * direction );
    }
    //End PID loop
    //setDrive(-10 * direction, -10 * direction);
    pros::delay(5);
    setDrive(0, 0);
    //Old deceleration math, no PID
/*
    //Lower drive speed as it approaches target distance
    if (getAvgEncoder() > abs(units) * 0.95) {
      setDrive(voltage1 * 0.1 * direction, voltage2 * 0.1 * direction);
    }
    else if (getAvgEncoder() > abs(units) * 0.9) {
      setDrive(voltage1 * 0.2 * direction, voltage2 * 0.2 * direction);
    }
    else if (getAvgEncoder() > abs(units) * 0.8) {
      setDrive(voltage1 * 0.3 * direction, voltage2 * 0.3 * direction);
    }
    else if (getAvgEncoder() > abs(units) * 0.7) {
      setDrive(voltage1 * 0.5 * direction, voltage2 * 0.5 * direction);
    }
    else if (getAvgEncoder() > abs(units) * 0.6) {
      setDrive(voltage1 * 0.8 * direction, voltage2 * 0.8 * direction);
    }
    else if (getAvgEncoder() > abs(units) * 0.5) {
      setDrive(voltage1 * 0.95 * direction, voltage2 * 0.95 * direction);
    }
    else {
      setDrive(voltage1 * direction, voltage2 * direction);
    }
*/
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

