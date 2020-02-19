#include "main.h"

// Default values
bool isAuton = false;

void SetStack() {
  Roller.moveVelocity(-60);
  while (AnglerAngle.get() <= 83) {
    AnglerMotor.moveVelocity(80);
  }
  AnglerMotor.moveVelocity(0);
  pros::delay(500);
  chassis->moveDistance(-4_in);
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
  chassis->moveDistance(-4_in);
  chassis->moveDistance(4_in);
  Deploy();
}

void blueSmall() {
  Deploy();
  driveTrain->right(-80);
  driveTrain->left(-50);
  pros::delay(650);
  LiftMotor.moveVelocity(0);
  
  Roller.moveVelocity(200);
  chassis->setMaxVelocity(90);
  chassis->moveDistance(1.3_ft);
  chassis->setMaxVelocity(130);
  chassis->turnAngle(15_deg);
  chassis->moveDistance(5_in);
  chassis->moveDistance(-11_in);
  chassis->turnAngle(-98_deg);
  chassis->moveDistance(7.5_in);
  Roller.moveVelocity(0);

  SetStack();
}

void redSmall() {
  chassis->setTurnsMirrored(true);
  Deploy();
  driveTrain->right(-80);
  driveTrain->left(-50);
  pros::delay(650);
  LiftMotor.moveVelocity(0);
  
  Roller.moveVelocity(200);
  chassis->setMaxVelocity(90);
  chassis->moveDistance(1.3_ft);
  chassis->setMaxVelocity(130);
  chassis->turnAngle(11_deg);
  chassis->moveDistance(5_in);
  chassis->moveDistance(-11.5_in);
  chassis->turnAngle(-104_deg);
  chassis->moveDistance(7.2_in);
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
  driveTrain->right(-80);
  driveTrain->left(-50);
  pros::delay(650);
  LiftMotor.moveVelocity(0);
  
  Roller.moveVelocity(200);
  chassis->setMaxVelocity(90);
  chassis->moveDistance(1.3_ft);
  chassis->setMaxVelocity(130);
  chassis->turnAngle(15_deg);
  chassis->moveDistance(5_in);
  chassis->moveDistance(-11_in);
  chassis->turnAngle(-98_deg);
  chassis->moveDistance(7.5_in);
  Roller.moveVelocity(0);

  SetStack();
  
  chassis->moveDistanceAsync(-1_ft);
  while (AnglerAngle.get() <= 5) {
    AnglerMotor.moveVelocity(-80);
  }
  AnglerMotor.moveVelocity(0);
  pros::delay(200);
  chassis->turnAngle(35_deg);
  Roller.moveVelocity(100);
  chassis->moveDistance(1.5_ft);
  pros::delay(500);
  Roller.moveVelocity(0);
  chassis->moveDistance(-6_in);
  LiftMotor.moveRelative(3000, 100);
  pros::delay(2000);
  Roller.moveRelative(-1000, 100);
  chassis->moveDistance(3_in);
  chassis->moveDistance(6_in);
  LiftMotor.setBrakeMode(AbstractMotor::brakeMode::coast);
  LiftMotor.moveVoltage(0);
  
}


// Part of the auton selector code
void (*scripts[])() = {&redLarge, &redSmall, &blueLarge, &blueSmall};
void LCDScriptExecute() { scripts[selectedAuton - 1](); }
void autonomous() {
  isAuton = true;
  lv_tabview_set_tab_act(tabview, 2, true);
  
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