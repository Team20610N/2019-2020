#include "main.h"

// master controller declaration
Controller master (ControllerId::master);
Controller partner (ControllerId::partner);

Motor LiftMotor(11, false, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);
Motor RightRollerMotor(18, true, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor LeftRollerMotor(20, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor AnglerMotor(12, false, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);
MotorGroup Roller({-18, 20});

ADIEncoder AnglerAngle('A', 'B', true);
int startingAnglerAngle;

void opcontrol() {
  isAuton = false;
  lv_tabview_set_tab_act(tabview, 3, true);
  chassis->setTurnsMirrored(false);
  // Seting the motor brake mode to hold
  LiftMotor.setBrakeMode(AbstractMotor::brakeMode::hold);
  RightRollerMotor.setBrakeMode(AbstractMotor::brakeMode::hold);
  LeftRollerMotor.setBrakeMode(AbstractMotor::brakeMode::hold);
  
  // Creating controller buttons
  ControllerButton leftScreenButton(ControllerDigital::up);
  ControllerButton rightScreenButton(ControllerDigital::left);
  
  ControllerButton TowerIntakeButton(ControllerDigital::right);
  ControllerButton intakeInButton(ControllerDigital::L1); 
  ControllerButton intakeOutButton(ControllerDigital::L2);
  ControllerButton anglerDownButton(ControllerDigital::R1);
  ControllerButton anglerUpButton(ControllerDigital::R2);
  
  // veriables for braking
  bool rollersLock = false;
  bool liftLock = false;
  bool anglerLock = false;

  // Start the thread for the base
  pros::Task ChassisOpcontrol_TR(ChassisOpcontrol, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "My Task");
  while (true) {
    updateLineVariable(2, AnglerAngle.get());
    lv_chart_set_next(chart, SilverLine, LeftRollerMotor.getTemperature());
    lv_chart_set_next(chart, WhiteLine, RightRollerMotor.getTemperature());
    lv_chart_set_next(chart, PurpleLine, AnglerMotor.getTemperature());
    lv_chart_set_next(chart, OrangeLine, LiftMotor.getTemperature());
    
    // Cycles the active screen tab using the controller
    if (leftScreenButton.changedToPressed()) {lv_tabview_set_tab_act(tabview, (lv_tabview_get_tab_act(tabview) + 1 + 3) % 3, false);}
    else if (rightScreenButton.changedToPressed()) {lv_tabview_set_tab_act(tabview, (lv_tabview_get_tab_act(tabview) - 1 + 3) % 3, false);}


    /* 
      Roller controls
      Master: 
        Digital L1 L2
      Partner:
        Analog leftY
        Digital R1 and R2
    */
    
    // Roller holds it's position as long as the tray isn't up.
    if (AnglerAngle.get() <= (startingAnglerAngle+10)) {
      Roller.setBrakeMode(AbstractMotor::brakeMode::coast);
    }
    else {
      Roller.setBrakeMode(AbstractMotor::brakeMode::hold);
      rollersLock = false;
    }
    
    // Controller Button maps
    if (abs(partner.getAnalog(ControllerAnalog::leftY)) > 0.1) {
      RightRollerMotor.moveVelocity(partner.getAnalog(ControllerAnalog::leftY)*200);
      LeftRollerMotor.moveVelocity(partner.getAnalog(ControllerAnalog::leftY)*200);
      rollersLock = false;
    }
    else if (intakeInButton.isPressed() || partner.getDigital(ControllerDigital::R1)) {
      RightRollerMotor.moveVelocity(200);
      LeftRollerMotor.moveVelocity(200);
      rollersLock = false;
    }
    else if (intakeOutButton.isPressed()) {
      RightRollerMotor.moveVelocity(-200);
      LeftRollerMotor.moveVelocity(-200);
      rollersLock = false;
    }
    else if (TowerIntakeButton.isPressed() || partner.getDigital(ControllerDigital::R2)) {
      RightRollerMotor.moveVelocity(-100);
      LeftRollerMotor.moveVelocity(-100);
      rollersLock = false;
    }
    // Roller Lock
    else if (rollersLock == false) {
      RightRollerMotor.moveRelative(0, 200);
      LeftRollerMotor.moveRelative(0, 200);
      rollersLock = true;
    }
    
    if (partner.getDigital(ControllerDigital::left)) {
      startingAnglerAngle -= startingAnglerAngle;
    }
    
    
    /*
      Angler Controls
      Master:
      
      Partner:
      
    */
    // Angler Lock
    if (anglerLock == false) {
        if (AnglerAngle.get() >= (startingAnglerAngle+20)) {
          AnglerMotor.moveRelative(0, 50);
        }
        else {
          AnglerMotor.moveVoltage(0);
        }
      anglerLock = true;
    }
    
    // If the tray is back, stop the motor
    if (AnglerAngle.get() <= (startingAnglerAngle+10)) {
      if (partner.getDigital(ControllerDigital::L1) || master.getDigital(ControllerDigital::R1)) {
        AnglerMotor.moveVelocity(50);
        anglerLock = false;
      }
      else {
        AnglerMotor.moveVoltage(0);
      }
    }
    // Prevent over extension
    else if (AnglerAngle.get() >= 110) {
      if (partner.getDigital(ControllerDigital::L2) || master.getDigital(ControllerDigital::R2)) {
        AnglerMotor.moveVelocity(-50);
        anglerLock = false;
      }
    }
    else {
      if (partner.getDigital(ControllerDigital::L2) || master.getDigital(ControllerDigital::R2)) {
        AnglerMotor.moveVelocity(-50);
        anglerLock = false;
      }
      else if (master.getDigital(ControllerDigital::R1) || partner.getDigital(ControllerDigital::L1)) {
        AnglerMotor.moveVelocity(50);
        Roller.moveVelocity(-AnglerMotor.getActualVelocity() / 1.9);
        rollersLock = false;
        anglerLock = false;
      }
    }
    
    
    /*
      Lift Controls
      Master:
        Analog rightY
      Partner:
        Analog rightY
    */
    updateLineVariable(2, LiftMotor.getPosition());
    if (abs(partner.getAnalog(ControllerAnalog::rightY)) > 0.3) {
      LiftMotor.moveVelocity(partner.getAnalog(ControllerAnalog::rightY)*200);
      AnglerMotor.moveVelocity(LiftMotor.getActualVelocity() / 1.4);
      liftLock = false;
      anglerLock = false;
    }
    else if (abs(master.getAnalog(ControllerAnalog::rightY)) > 0.6) {
      LiftMotor.moveVelocity(master.getAnalog(ControllerAnalog::rightY)*200);
      AnglerMotor.moveVelocity(LiftMotor.getActualVelocity() / 1.4);
      liftLock = false;
      anglerLock = false;
    }
    else if (LiftMotor.getPosition() <= 100) {
      LiftMotor.moveVoltage(0);
    }
    // Lift Lock
    else if (liftLock == false) {
      if (LiftMotor.getPosition() >= 100) { 
        LiftMotor.moveRelative(0, 20);
      }
      else {
        LiftMotor.moveVoltage(0);
      }
      liftLock = true;
    }
    
    // Tray Deploy for driver
    if (partner.getDigital(ControllerDigital::up)) {
      Deploy();
    }
    
    
    // Stack release
    if (partner.getDigital(ControllerDigital::down) || master.getDigital(ControllerDigital::X)) {
      ChassisOpcontrol_TR.suspend();
      Roller.moveVelocity(-125);
      pros::delay(150);
      driveTrain->forward(-200);
      pros::delay(400);
      isAuton = false;
      ChassisOpcontrol_TR.resume();
      Roller.moveVelocity(0);
    }
    pros::delay(20);
  }
}
