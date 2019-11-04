#include "main.h"

// master controller declaration
Controller master (ControllerId::master);

void opcontrol() {
  pros::Task ChassisOpcontrol_TR(ChassisOpcontrol, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "My Task");
  // Sets the drivetrain to hold
  driveTrain->setBrakeMode(AbstractMotor::brakeMode::hold);

  while (true) {
    // updates the drive train values.
    // driveTrain->hArcade(master.getAnalog(ControllerAnalog::leftX), master.getAnalog(ControllerAnalog::leftY), master.getAnalog(ControllerAnalog::rightX), 0.3);
    pros::delay(20);
  }
}