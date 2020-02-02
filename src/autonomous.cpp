#include "main.h"

// Default values
bool isAuton = false;


void redSmall() {
  Roller.moveRelative(-2000, -200);
  pros::delay(1000);
  LiftMotor.moveRelative(300, 100);
  pros::delay(1000);
  Roller.moveVelocity(190);
  LiftMotor.moveRelative(-400, -100);
  pros::delay(500);
  
  
  LiftMotor.moveRelative(0, 200);
  chassis->moveDistance(1.3_ft);
  pros::delay(200);
  Roller.moveVelocity(0);
  chassis->moveDistance(-0.8_ft);
  chassis->turnAngle(62_deg);
  chassis->moveDistance(0.435_ft);
  
  
  Roller.moveRelative(-700, -40);
  pros::delay(500);
  AnglerMotor.moveRelative(-800, 50);
  pros::delay(2000);
  Roller.moveVelocity(-100);
  chassis->setMaxVelocity(100);
  chassis->moveDistance(1_in);
  pros::delay(200);
  AnglerMotor.moveRelative(600, 50);
  pros::delay(2500);
  chassis->setMaxVelocity(60);
  chassis->moveDistance(-0.3_ft);
  Roller.moveVelocity(0);
}

void blueLarge() {
  chassis->setMaxVelocity(150);
  Roller.moveRelative(-2000, -200);
  pros::delay(1000);
  LiftMotor.moveRelative(300, 100);
  pros::delay(500);
  pros::delay(500);
  LiftMotor.moveRelative(-300, -100);
  pros::delay(1000);
  
  Roller.moveVelocity(100);
  chassis->moveDistance(5_in);
  chassis->turnAngle(53_deg);
  Roller.moveVelocity(-200);
  pros::delay(500);
  
  chassis->moveDistance(-2_in);
  Roller.moveVelocity(70);
  chassis->turnAngle(63_deg);
  chassis->setMaxVelocity(150);
  chassis->moveDistance(6_in);
  Roller.moveVelocity(0);
  AnglerMotor.moveRelative(-350, -50);
  LiftMotor.moveRelative(600, 100);
  
  pros::delay(500);
  chassis->setMaxVelocity(100);
  chassis->moveDistance(6.25_in);
  // chassis->turnAngle(-10_deg);
  Roller.moveVelocity(-80);
  pros::delay(1000);
  chassis->moveDistance(-3_in);
  AnglerMotor.moveVoltage(0);
  LiftMotor.moveVoltage(0);
  Roller.moveVoltage(0);
}

void redLarge() {
  chassis->setTurnsMirrored(true);
  blueLarge();
  chassis->setTurnsMirrored(false);
} 

void blueSmall() {
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