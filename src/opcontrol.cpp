#include "main.h"

// chassis declaration
std::shared_ptr<ChassisController> chassis = ChassisControllerBuilder()
  .withMotors(10, -9, 8)
  .withGearset(AbstractMotor::gearset::green)
  .withDimensions({{4_in, 10_in}, imev5GreenTPR})
  .build();

void opcontrol() {
  // assigning the chassis to a H-drive model
  auto driveTrain = std::dynamic_pointer_cast<HDriveModel>(chassis->getModel());
  
  // master controller declaration
  Controller controller;
  
  // Sets the drivetrain to hold
  driveTrain->setBrakeMode(AbstractMotor::brakeMode::hold);

  while (true) {
    // updates the drive train values.
    driveTrain->hArcade(controller.getAnalog(ControllerAnalog::leftX), controller.getAnalog(ControllerAnalog::leftY),controller.getAnalog(ControllerAnalog::rightX), 0.3);
    pros::delay(20);
  }
}