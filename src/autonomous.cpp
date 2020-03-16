#include "main.h"

// When set to true driver code stops
bool isAuton = false;

void SetStack() {
  Roller.moveVelocity(-60);
  while (AnglerAngle.get() <= 83) {
    AnglerMotor.moveVelocity(80);
  }
  AnglerMotor.moveVelocity(0);
  pros::delay(200);
  chassis->moveDistance(-8_in);
  Roller.moveVoltage(0);
}

void SetStackSafe() {
  Roller.moveVelocity(-60);
  pros::delay(550);
  Roller.moveVelocity(0);
  while (AnglerAngle.get() <= 85) {
    AnglerMotor.moveVelocity(50);
    if (AnglerAngle.get() >= 35){
      Roller.moveVelocity(-AnglerMotor.getActualVelocity() / 1.9);
    }
  }
  Roller.moveVelocity(-130);
  AnglerMotor.moveVelocity(0);
  pros::delay(200);
  chassis->setMaxVelocity(100);
  chassis->moveDistance(-8_in);
  Roller.moveVoltage(0);
}

void Deploy() {
  LiftMotor.moveVelocity(200);
  pros::delay(500);
  Roller.moveVelocity(-100);
  pros::delay(900);
  LiftMotor.setBrakeMode(AbstractMotor::brakeMode::coast);
  LiftMotor.moveVoltage(0);
  Roller.moveVoltage(0);
  pros::delay(1000);
}

void blueLarge() {
  chassis->moveDistance(-10_in);
  chassis->moveDistance(10_in);
  Deploy();
}

void blueSmall() {
  Deploy();
  driveTrain->xArcade(50, 0, 0, 0.3);
  pros::delay(500);
  driveTrain->xArcade(0, -30, 0, 0.3);
  pros::delay(400);
  driveTrain->xArcade(0, 0, 0, 0.3);
  Roller.moveVelocity(200);
  chassis->setMaxVelocity(100);
  chassis->moveDistance(3.6_ft);
  chassis->turnAngle(25_deg);
  chassis->moveDistance(1_ft);
  chassis->setMaxVelocity(150);
  chassis->moveDistanceAsync(-2_ft);
  pros::delay(1200);
  Roller.moveVelocity(0);
  chassis->turnAngle(183_deg);
  Roller.moveVelocity(200);
  chassis->setMaxVelocity(130);
  chassis->moveDistance(2.1_ft);
  Roller.moveVelocity(0);

  SetStack();
}

void redSmall() {
  chassis->setTurnsMirrored(true);
    Deploy();
  driveTrain->xArcade(-50, 0, 0, 0.3);
  pros::delay(500);
  driveTrain->xArcade(0, -30, 0, 0.3);
  pros::delay(300);
  driveTrain->xArcade(0, 0, 0, 0.3);
  Roller.moveVelocity(200);
  chassis->setMaxVelocity(100);
  chassis->moveDistance(3.6_ft);
  chassis->turnAngle(25_deg);
  chassis->moveDistance(1_ft);
  chassis->setMaxVelocity(150);
  chassis->moveDistanceAsync(-2.5_ft);
  pros::delay(1200);
  Roller.moveVelocity(0);
  chassis->turnAngle(176_deg);
  Roller.moveVelocity(200);
  chassis->moveDistance(1.8_ft);
  Roller.moveVelocity(0);

  SetStack();
  chassis->setTurnsMirrored(false);
}

void redLarge() {
  chassis->setTurnsMirrored(true);
  blueLarge();
  chassis->setTurnsMirrored(false);
} 

void Skills () {
  Deploy();
  chassis->setMaxVelocity(100);
  Roller.moveVelocity(200);
  driveTrain->forward(-50);
  pros::delay(500);
  chassis->moveDistance(4_ft);
  chassis->turnAngle(28_deg);
  chassis->moveDistance(-4.2_ft);
  Roller.moveVelocity(0);
  chassis->turnAngle(-34_deg);
  
  // Get row of 4 cubes
  Roller.moveVelocity(200);
  chassis->setMaxVelocity(70);
  chassis->moveDistance(3.5_ft);
  pros::delay(400);
  Roller.moveVelocity(-50);
  pros::delay(400);
  Roller.moveVelocity(100);
  chassis->moveDistance(-2.2_ft);
  Roller.moveVelocity(0);
  
  // turn and set stack
  chassis->setMaxVelocity(100);
  chassis->turnAngle(-127_deg);
  chassis->moveDistance(1.6_ft);
  SetStackSafe();
  
  // drive to first tower
  chassis->moveDistance(-1_ft);
  chassis->turnAngle(-134_deg);
  chassis->setMaxVelocity(80);
  chassis->moveDistanceAsync(3.2_ft);
  while (AnglerAngle.get() >= 5) {
    AnglerMotor.moveVelocity(-200);
  }
  AnglerMotor.moveVoltage(0);
  Roller.moveVelocity(170);
  pros::delay(700);
  Roller.moveVelocity(0);
  chassis->moveDistance(0_ft);
  // pros::delay(100);
  chassis->setMaxVelocity(100);
  chassis->moveDistance(-6_in);
  
  // Put cube in first tower
  // Roller.moveRelative(-900, -200);
  pros::delay(300);
  LiftMotor.moveRelative(600, 100);
  while (AnglerAngle.get() <= 35) {
    AnglerMotor.moveVelocity(80);
  }
  AnglerMotor.moveVoltage(0);
  pros::delay(600);
  chassis->moveDistance(10_in);
  Roller.moveVelocity(-70);
  pros::delay(500);
  chassis->moveDistance(-4_in);
  LiftMotor.moveVoltage(0);
  
  // turn get preload cube drive to second tower
  chassis->turnAngle(80_deg);
  Roller.moveVelocity(200);
  chassis->moveDistance(9_in);
  chassis->moveDistance(-1.8_ft);
  Roller.moveVelocity(0);
  chassis->turnAngle(109_deg);
  chassis->moveDistance(2.3_ft);
  
  // setup for tower
  Roller.moveRelative(-1200, -200);
  pros::delay(300);
  LiftMotor.moveRelative(450, 100);
  while (AnglerAngle.get() <= 35) {
    AnglerMotor.moveVelocity(80);
  }
  AnglerMotor.moveVoltage(0);
  pros::delay(600);
  chassis->moveDistance(10_in);
  
  // place cube into second tower
  Roller.moveVelocity(-70);
  pros::delay(600);
  chassis->moveDistance(-10_in);
  LiftMotor.moveVoltage(0);
  Roller.moveVoltage(0);
  
  // get last cube for third tower
  chassis->moveDistance(-1_ft);
  chassis->turnAngle(93_deg);
  Roller.moveVelocity(200);
  chassis->moveDistance(1.8_ft);
  
  // set up for tower
  chassis->moveDistance(-10_in);
  Roller.moveRelative(-1200, -200);
  pros::delay(300);
  LiftMotor.moveRelative(450, 100);
  while (AnglerAngle.get() <= 35) {
    AnglerMotor.moveVelocity(80);
  }
  AnglerMotor.moveVoltage(0);
  pros::delay(600);
  chassis->moveDistance(10_in);
  
  // put last cube into tower
  Roller.moveVelocity(-70);
  pros::delay(600);
  chassis->moveDistance(-10_in);
  LiftMotor.moveVoltage(0);
  Roller.moveVoltage(0);
}

void redSafe() {
  chassis->setTurnsMirrored(true);
  Deploy();
  driveTrain->xArcade(-50, 0, 0, 0.3);
  pros::delay(500);
  driveTrain->xArcade(0, -30, 0, 0.3);
  pros::delay(300);
  driveTrain->xArcade(0, 0, 0, 0.3);
  Roller.moveVelocity(200);
  chassis->setMaxVelocity(90);
  chassis->moveDistance(3.3_ft);
  chassis->moveDistance(-1.8_ft);
  chassis->turnAngle(-135_deg);
  chassis->moveDistance(1.4_ft);
  SetStack();
  chassis->setTurnsMirrored(false);
}

void blueSafe() {
  Deploy();
  driveTrain->xArcade(50, 0, 0, 0.3);
  pros::delay(500);
  driveTrain->xArcade(0, -30, 0, 0.3);
  pros::delay(300);
  driveTrain->xArcade(0, 0, 0, 0.3);
  Roller.moveVelocity(200);
  chassis->setMaxVelocity(90);
  chassis->moveDistance(3.3_ft);
  chassis->moveDistance(-1.8_ft);
  chassis->turnAngle(-135_deg);
  chassis->moveDistance(1.4_ft);
  SetStack();
}

// Part of the auton selector code
void (*scripts[])() = {&redLarge, &redSmall, &blueLarge, &blueSmall};
void LCDScriptExecute() { scripts[selectedAuton - 1](); }
void autonomous() {
  isAuton = true;
  lv_tabview_set_tab_act(tabview, 2, true);
  
  switch (selectedAuton) {
    case 1: blueSmall();
      break;
    case 2: blueSafe();
      break;
    case 3: blueLarge();
      break;
    case 4: Skills();
      break;
    case 5: redLarge();
      break;
    case 6: redSafe();
      break;
    case 7: redSmall();
      break;
  }
  isAuton = false;
}