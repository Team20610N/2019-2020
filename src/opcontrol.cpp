#include "main.h"

// master controller declaration
Controller master (ControllerId::master);
Controller partner (ControllerId::partner);

void opcontrol() {
  Motor liftMotor(18, true, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
  Motor rightRollerMotor(19, true, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
  Motor leftRollerMotor(20, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
  
  // Sets the drivetrain to hold
  driveTrain->setBrakeMode(AbstractMotor::brakeMode::hold);
  liftMotor.setBrakeMode(AbstractMotor::brakeMode::hold);
  rightRollerMotor.setBrakeMode(AbstractMotor::brakeMode::hold);
  leftRollerMotor.setBrakeMode(AbstractMotor::brakeMode::hold);

  ControllerButton leftAutonButton(ControllerDigital::down);
  ControllerButton rightAutonButton(ControllerDigital::right);
  ControllerButton leftScreenButton(ControllerDigital::up);
  ControllerButton rightScreenButton(ControllerDigital::left);
  
  bool rollersLock = false;
  bool liftLock = false;

  pros::Task ChassisOpcontrol_TR(ChassisOpcontrol, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "My Task");
  while (true) {
    // changes the active auton using the controller
    if (leftAutonButton.changedToPressed()) {selectedAuton = (selectedAuton - 1 + 4) % 4;SetAutonButton(selectedAuton);}
    else if (rightAutonButton.changedToPressed()) {selectedAuton = (selectedAuton + 1 + 4) % 4;SetAutonButton(selectedAuton);}
    
    // changes the screen using the controller
    if (leftScreenButton.changedToPressed()) {lv_tabview_set_tab_act(tabview, (lv_tabview_get_tab_act(tabview) + 1 + 3) % 3, false);}
    else if (rightScreenButton.changedToPressed()) {lv_tabview_set_tab_act(tabview, (lv_tabview_get_tab_act(tabview) - 1 + 3) % 3, false);}
    
    // updates the drive train values.
    // driveTrain->hArcade(master.getAnalog(ControllerAnalog::leftX), master.getAnalog(ControllerAnalog::leftY), master.getAnalog(ControllerAnalog::rightX), 0.3);
    
    if (abs(partner.getAnalog(ControllerAnalog::leftY)) > 0.3) {
      rightRollerMotor.moveVelocity(partner.getAnalog(ControllerAnalog::leftY)*200);
      leftRollerMotor.moveVelocity(partner.getAnalog(ControllerAnalog::leftY)*200);
      rollersLock = false;
    }
    else if (rollersLock == false) {
      rightRollerMotor.moveRelative(0, 200);
      leftRollerMotor.moveRelative(0, 200);
      rollersLock = true;
    }
    
    if (abs(partner.getAnalog(ControllerAnalog::rightY)) > 0.3) {
      liftMotor.moveVelocity(partner.getAnalog(ControllerAnalog::rightY)*200);
      liftLock = false;
    }
    else if (liftLock == false) {
      liftMotor.moveRelative(0, 200);
      liftLock = true;
    }
    
    pros::delay(20);
  }
}