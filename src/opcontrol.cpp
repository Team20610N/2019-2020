#include "main.h"

// master controller declaration
Controller master (ControllerId::master);
Controller partner (ControllerId::partner);

Motor LeftMotor(2, true, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);
Motor RightRollerMotor(11, true, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor LeftRollerMotor(20, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor AnglerMotor(13, false, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);
MotorGroup Roller({-11, 20});

void opcontrol() {
  pros::delay(20000000);
  
  // Seting the motor brake mode to hold
  LeftMotor.setBrakeMode(AbstractMotor::brakeMode::hold);
  RightRollerMotor.setBrakeMode(AbstractMotor::brakeMode::hold);
  LeftRollerMotor.setBrakeMode(AbstractMotor::brakeMode::hold);
  
  // Creating controller buttons
  ControllerButton leftAutonButton(ControllerDigital::down);
  ControllerButton rightAutonButton(ControllerDigital::right);
  ControllerButton leftScreenButton(ControllerDigital::up);
  ControllerButton rightScreenButton(ControllerDigital::left);
  
  ControllerButton intakeInButton(ControllerDigital::L1); 
  ControllerButton intakeOutButton(ControllerDigital::L2);
  ControllerButton anglerInButton(ControllerDigital::R1);
  ControllerButton anglerOutButton(ControllerDigital::R2);
  
  // veriables for braking
  bool rollersLock = false;
  bool liftLock = false;
  bool anglerLock = false;

  // Start the thread for the base
  pros::Task ChassisOpcontrol_TR(ChassisOpcontrol, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "My Task");
  while (true) {
    // changes the active auton using the controller
    if (leftAutonButton.changedToPressed()) {selectedAuton = (selectedAuton - 1 + 4) % 4;SetAutonButton(selectedAuton);}
    else if (rightAutonButton.changedToPressed()) {selectedAuton = (selectedAuton + 1 + 4) % 4;SetAutonButton(selectedAuton);}
    
    // Sycles the active screen tab using the controller
    if (leftScreenButton.changedToPressed()) {lv_tabview_set_tab_act(tabview, (lv_tabview_get_tab_act(tabview) + 1 + 3) % 3, false);}
    else if (rightScreenButton.changedToPressed()) {lv_tabview_set_tab_act(tabview, (lv_tabview_get_tab_act(tabview) - 1 + 3) % 3, false);}

    if (abs(partner.getAnalog(ControllerAnalog::leftY)) > 0.3) {
      RightRollerMotor.moveVelocity(partner.getAnalog(ControllerAnalog::leftY)*200);
      LeftRollerMotor.moveVelocity(partner.getAnalog(ControllerAnalog::leftY)*200);
      rollersLock = false;
    }
    else if (intakeInButton.isPressed() || partner.getDigital(ControllerDigital::R1)) {
      RightRollerMotor.moveVelocity(100);
      LeftRollerMotor.moveVelocity(100);
      rollersLock = false;
    }
    else if (intakeOutButton.isPressed() || partner.getDigital(ControllerDigital::R2)) {
      RightRollerMotor.moveVelocity(-100);
      LeftRollerMotor.moveVelocity(-100);
      rollersLock = false;
    }
    else if (rollersLock == false) {
      RightRollerMotor.moveRelative(0, 200);
      LeftRollerMotor.moveRelative(0, 200);
      rollersLock = true;
    }
    
    if (partner.getDigital(ControllerDigital::L2) || anglerOutButton.isPressed()) {
      AnglerMotor.moveVelocity(50);
      anglerLock = false;
    }
    else if (partner.getDigital(ControllerDigital::L1) || anglerInButton.isPressed()) {
      AnglerMotor.moveVelocity(-50);
      anglerLock = false;
    }
    else if (anglerLock == false) {
      AnglerMotor.moveRelative(0, 50);
      anglerLock = true;
    }
    
    if (abs(partner.getAnalog(ControllerAnalog::rightY)) > 0.3) {
      LeftMotor.moveVelocity(partner.getAnalog(ControllerAnalog::rightY)*200);
      AnglerMotor.moveVelocity(-LeftMotor.getActualVelocity() / 1.8);
      liftLock = false;
      anglerLock = false;
    }
    else if (partner.getDigital(ControllerDigital::up)) {
      LeftMotor.moveVelocity(100);
      AnglerMotor.moveVelocity(-LeftMotor.getActualVelocity() / 1.8);
      liftLock = false;
      anglerLock = false;
    }
    else if (partner.getDigital(ControllerDigital::down)) {
      LeftMotor.moveVelocity(-100);
      AnglerMotor.moveVelocity(-LeftMotor.getActualVelocity() / 1.8);
      liftLock = false;
      anglerLock = false;
    }
    else if (abs(master.getAnalog(ControllerAnalog::rightY)) > 0.6) {
      LeftMotor.moveVelocity(master.getAnalog(ControllerAnalog::rightY)*200);
      AnglerMotor.moveVelocity(-LeftMotor.getActualVelocity() / 1.8);
      liftLock = false;
      anglerLock = false;
    }
    else if (liftLock == false) {
      LeftMotor.moveRelative(0, 100);
      liftLock = true;
    }
    
    pros::delay(20);
  }
}
