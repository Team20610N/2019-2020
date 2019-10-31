#include "main.h"

void opcontrol() {
  // chassis declaration
  auto chassis = ChassisControllerBuilder()
  .withMotors(10, -9)                    
  .withGearset(AbstractMotor::gearset::green)
  .withDimensions({{4_in, 10_in}, imev5GreenTPR})
  .build();
  
  // assigning the chassis to a skidsteermodel
  auto driveTrain = std::dynamic_pointer_cast<SkidSteerModel>(chassis->getModel());
  
  // Motor declaration
  Motor Straff(8);
  
  // master controller declaration
  Controller controller;
  
  // Sets the drivetrain and the straffing motor to hold
  driveTrain->setBrakeMode(AbstractMotor::brakeMode::hold);
  Straff.setBrakeMode(AbstractMotor::brakeMode::hold);

  while (true) {
    // updates the drive train values.
    driveTrain->arcade(controller.getAnalog(ControllerAnalog::leftY),controller.getAnalog(ControllerAnalog::rightX), 0.3);
    
    // Straffing motor control with dead zones.
    if (abs(controller.getAnalog(ControllerAnalog::leftX)) >= 0.4) {Straff.moveVelocity(controller.getAnalog(ControllerAnalog::leftX) * 200);}
    else {Straff.moveVelocity(0);}
    pros::delay(20);
  }
}