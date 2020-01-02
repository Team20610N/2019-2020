#include "main.h"

// Default values
bool isAuton = false;

void redLarge() {
  Roller.moveVelocity(100);
  pros::delay(2000);
  Roller.moveVelocity(0);
  // Roller.moveVelocity(100);
  // pros::delay(500);
  // Roller.moveVelocity(0);
  // chassis->moveDistance(-1_ft);
  // chassis->turnAngle(90);
  // Roller.moveVelocity(50);
  // chassis->moveDistance(1_ft);
  // Roller.moveVelocity(0);
  // chassis->turnAngle(90);
  // //Move arm up and tray
  // chassis->moveDistanceAsync(1_ft);
  // Roller.moveVelocity(-50);
  // pros::delay(500);
  // Roller.moveVelocity(0);
  // chassis->moveDistance(-1_ft);
} 
void redSmall() {
  Roller.moveVelocity(100);
  pros::delay(2000);
  Roller.moveVelocity(0);
  // Roller.moveVelocity(150);
  // chassis->moveDistance(4_ft);
  // Roller.moveVelocity(0);
  // chassis->moveDistance(-3.5_ft);
  // chassis->turnAngle(90_deg);
  // chassis->moveDistance(0.25_ft);
  
  // AnglerMotor.moveRelative(1000, 100);
  
  // chassis->moveDistance(-1);
}
void blueLarge() {
  Roller.moveVelocity(100);
  pros::delay(2000);
  Roller.moveVelocity(0);
}
void blueSmall() {
  Roller.moveVelocity(100);
  pros::delay(2000);
  Roller.moveVelocity(0);
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