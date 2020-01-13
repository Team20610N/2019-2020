#include "main.h"

// Default values
bool isAuton = false;

void redLarge() {
  Roller.moveVelocity(80);
  chassis->moveDistance(2_ft);
  Roller.moveVelocity(0);
  chassis->turnAngle(-150_deg);
  chassis->moveDistanceAsync(0.9_ft);
  pros::delay(400);
  Roller.moveVelocity(-200);
  pros::delay(1000);
  Roller.moveVelocity(0);
  
  // chassis->moveDistance(-2.4_ft);
  leftMotor.moveRelative(-4600, 100);
  rightMotor.moveRelative(-4600, 100);
  pros::delay(2000);
  chassis->turnAngle(135_deg);
  Roller.moveVelocity(200);
  leftMotor.moveRelative(1500, 100);
  rightMotor.moveRelative(1500, 100);
  pros::delay(1000);
  Roller.moveVelocity(0);
  
  chassis->moveDistance(-8_in);
  Roller.moveRelative(-500, 100);
  LiftMotor.moveVelocity(100);
  AnglerMotor.moveVelocity(-30);
  pros::delay(1800);
  LiftMotor.moveRelative(0, 200);
  AnglerMotor.moveRelative(0, 200);
  chassis->moveDistance(5_in);
  Roller.moveVelocity(-100);
  pros::delay(900);
  leftMotor.moveRelative(-1000, 100);
  rightMotor.moveRelative(-1000, 100);
  pros::delay(1000);
  Roller.moveVelocity(0);
  LiftMotor.moveRelative(0, 0);
  AnglerMotor.moveRelative(0, 0);
} 

void redSmall() {
  LiftMotor.moveRelative(0, 200);
  Roller.moveVelocity(120);
  chassis->moveDistance(3.8_ft);
  pros::delay(300);
  Roller.moveVelocity(0);
  chassis->moveDistance(-1.8_ft);
  chassis->turnAngle(140_deg);
  chassis->moveDistance(1.4_ft);
  Roller.moveRelative(-1000, 40);
  
  AnglerMotor.moveRelative(-500, 50);
  pros::delay(2000);
  chassis->moveDistance(2_in);
  chassis->moveDistance(-1_ft);
}

void blueLarge() {
  chassis->setTurnsMirrored(true);
Roller.moveVelocity(80);
  chassis->moveDistance(2_ft);
  Roller.moveVelocity(0);
  chassis->turnAngle(-150_deg);
  chassis->moveDistanceAsync(0.9_ft);
  pros::delay(400);
  Roller.moveVelocity(-200);
  pros::delay(1000);
  Roller.moveVelocity(0);
  
  // chassis->moveDistance(-2.4_ft);
  leftMotor.moveRelative(-4600, 100);
  rightMotor.moveRelative(-4600, 100);
  pros::delay(2000);
  chassis->turnAngle(135_deg);
  Roller.moveVelocity(200);
  leftMotor.moveRelative(1500, 100);
  rightMotor.moveRelative(1500, 100);
  pros::delay(1000);
  Roller.moveVelocity(0);
  
  chassis->moveDistance(-8_in);
  Roller.moveRelative(-500, 100);
  LiftMotor.moveVelocity(100);
  AnglerMotor.moveVelocity(-30);
  pros::delay(1800);
  LiftMotor.moveRelative(0, 200);
  AnglerMotor.moveRelative(0, 200);
  chassis->moveDistance(5_in);
  Roller.moveVelocity(-100);
  pros::delay(900);
  leftMotor.moveRelative(-1000, 100);
  rightMotor.moveRelative(-1000, 100);
  pros::delay(1000);
  Roller.moveVelocity(0);
  LiftMotor.moveRelative(0, 0);
  AnglerMotor.moveRelative(0, 0);
  chassis->setTurnsMirrored(false);
}

void blueSmall() {
  // chassis->moveDistance(1_ft);
  // chassis->setMaxVelocity(100);
  // chassis->moveDistance(1_ft);
  chassis->setTurnsMirrored(true);
  redSmall();
  chassis->setTurnsMirrored(false);
}

// Part of the auton selector code
void (*scripts[])() = {&redLarge, &redSmall, &blueLarge, &blueSmall};
void LCDScriptExecute() { scripts[selectedAuton - 1](); }
void autonomous() {
  isAuton = true;
  
  switch (selectedAuton) {
    case 1: blueLarge();
      break;
    case 2: blueSmall();
      break;
    case 3: redLarge();
      break;
    case 4: redSmall();
      break;
  }
  isAuton = false;
}