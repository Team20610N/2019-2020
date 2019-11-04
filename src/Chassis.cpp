#include "main.h"
// #include "fstream"

ADIEncoder leftEncoder('C', 'D');
ADIEncoder rightEncoder('E', 'F');

// chassis declaration
std::shared_ptr<ChassisController> chassis = ChassisControllerBuilder()
  .withMotors(10, -9, 8)
  .withGearset(AbstractMotor::gearset::green)
  // .withSensors(leftEncoder, rightEncoder)
  .withDimensions({{4_in, 10_in}, imev5GreenTPR})
  .build();

// assigning the chassis to a H-drive model
std::shared_ptr<okapi::HDriveModel> driveTrain = std::dynamic_pointer_cast<HDriveModel>(chassis->getModel());

int newHeading = 0;
int JoisticHeading = 0;
int chassisHeading;

// DELETE ME AFTER ADDING THE SCREEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
bool isAuton = false;

// Thread that controls the chassis 
void ChassisOpcontrol(void* param) {
    int leftMotor = 0;
    int rightMotor = 0;
    int middleMotor = 0;
    int forward, straff, turning;
    int temp;
    float theta;
    bool isStopped = false;
    bool FieldCenteric = true;
    
    ControllerButton fieldCentericToggle(ControllerDigital::A);
    ControllerButton gyroReset(ControllerDigital::X);
    
    ADIGyro gyro('A',1);

  while (isAuton == false) {
    chassisHeading = gyro.controllerGet();
    newHeading = chassisHeading - JoisticHeading;
    if (fieldCentericToggle.changedToPressed()) {
      if (FieldCenteric == true) {
        FieldCenteric = false;
      }
      else {
        FieldCenteric = true;
      }
    }

    forward = master.getAnalog(ControllerAnalog::leftY);
    if (abs(forward) < 3) {
      forward = 0;
    }
    straff = master.getAnalog(ControllerAnalog::leftX);
    if (abs(straff) < 3) {
      straff = 0;
    }

    turning = master.getAnalog(ControllerAnalog::rightX);
    if (abs(turning) < 3) {
      turning = 0;
    }


    theta = (-chassisHeading * 3.1415926535) / 180;
    temp  = forward * cos(theta) - straff * sin(theta);
    straff = forward * sin(theta) + straff * cos(theta);
    forward = temp;

    leftMotor = forward + turning;
    middleMotor = straff;
    rightMotor = forward - turning;

    int max = leftMotor;
    if (middleMotor > max) {
      max = middleMotor;
    }
    if (rightMotor > max) {
      max = rightMotor;
    }

    const int MaxSpeed = 200;
    if (max > MaxSpeed) {
      leftMotor = MaxSpeed * leftMotor / max;
      middleMotor = MaxSpeed * middleMotor / max;
      rightMotor = MaxSpeed * rightMotor / max;
    }

    if (forward == 0 && straff == 0 && turning == 0 && isStopped == false) {
      // Brake motors
      isStopped = true;
    }
    else {
      if (FieldCenteric == true) {
        if (turning > 3 || turning < -3) {
          driveTrain->left(leftMotor);
          driveTrain->middle(middleMotor);
          driveTrain->right(rightMotor);
          JoisticHeading = chassisHeading;
          isStopped = false;
            }
        else {
          driveTrain->left(leftMotor - (newHeading * 3));
          driveTrain->middle(middleMotor + (newHeading * 3));
          driveTrain->right(rightMotor + (newHeading * 3));
          isStopped = false;
        }
        }
      else {
        driveTrain->hArcade(master.getAnalog(ControllerAnalog::leftX), 
                            master.getAnalog(ControllerAnalog::leftY), 
                            master.getAnalog(ControllerAnalog::rightX), 0.3);
        isStopped = false;
      }
    }

  if (gyroReset.changedToPressed()) {
    gyro.reset();
    JoisticHeading = 0;
  }
    pros::delay(20);
  }
}