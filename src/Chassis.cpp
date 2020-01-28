#include "main.h"

ADIEncoder leftEncoder('E', 'F');
ADIEncoder rightEncoder('C', 'D');
ADIGyro Gyro('B',1);
pros::Imu IMU(8);

int newHeading = 0;
int JoisticHeading = 0;
int chassisHeading;
int headingError;
bool chassisBrake = false;

Motor frontLeftMotor(9, false, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);
Motor backLeftMotor(10, false, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);
Motor frontRightMotor(2, true, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);
Motor backRightMotor(1, true, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);
std::shared_ptr<ChassisController> chassis = ChassisControllerBuilder()
  .withMotors(frontLeftMotor, frontRightMotor, backRightMotor, backLeftMotor)
  .withSensors(leftEncoder, rightEncoder)
  .withDimensions(AbstractMotor::gearset::green, {{4_in, 10_in}, imev5GreenTPR})
  .withMaxVelocity(100)
  .withLogger(
      std::make_shared<Logger>(
          TimeUtilFactory::createDefault().getTimer(), // It needs a Timer
          "/usd/Chassis_Diagnostics", // Output to the SDCard
          Logger::LogLevel::debug // Most verbose log level
      )
  )
  .build();

// Thread that controls the chassis 
void ChassisOpcontrol(void* param) {
  // assigning the chassis to a X-drive model
  std::shared_ptr<okapi::XDriveModel> driveTrain = std::dynamic_pointer_cast<XDriveModel>(chassis->getModel());
  driveTrain->setBrakeMode(AbstractMotor::brakeMode::hold);
  
  int leftMotorControl = 0;
  int rightMotorControl = 0;
  int middleMotorControl = 0;
  double forward, straff, turning;
  int temp;
  float theta;
  bool FieldCenteric = true;
  
  ControllerButton fieldCentericToggle(ControllerDigital::A);
  ControllerButton gyroReset(ControllerDigital::X);

  while (isAuton == false) {
    chassisHeading = IMU.get_heading() -headingError;
    newHeading = chassisHeading - JoisticHeading;

    //Updates display values.
		updateLineVariable(1, chassisHeading);
		updateLineVariable(2, JoisticHeading);
		updateLineVariable(3, leftEncoder.get());
		updateLineVariable(4, rightEncoder.get());
    
    // Graphs the drive motor temps
    lv_chart_set_next(chart, GreenLine, frontLeftMotor.getTemperature());
    lv_chart_set_next(chart, LimeLine, frontRightMotor.getTemperature());
    
    forward = master.getAnalog(ControllerAnalog::leftY)*100;
    if (abs(forward) < 0.3) {
      forward = 0;
    }
    straff = master.getAnalog(ControllerAnalog::leftX)*100;
    if (abs(straff) < 0.3) {
      straff = 0;
    }
    turning = master.getAnalog(ControllerAnalog::rightX)*100;
    if (abs(turning) < 0.3) {
      turning = 0;
    }

    theta = (-chassisHeading * 3.1415926535) / 180;
    temp  = forward * cos(theta) - straff * sin(theta);
    straff = forward * sin(theta) + straff * cos(theta);
    forward = temp;
    
    if (fieldCentericToggle.changedToPressed()) {
      if (FieldCenteric == true) {
        FieldCenteric = false;
      }
      else {
        FieldCenteric = true;
      }
    }
    
    if (gyroReset.changedToPressed()) {
      Gyro.reset();
      headingError = IMU.get_heading();
    }
    
    if (FieldCenteric == true) {
      driveTrain->xArcade((straff / 100),
                          (forward / 100),
                          (turning / 100), 0);
    }
    else {
      driveTrain->xArcade(master.getAnalog(ControllerAnalog::leftX), 
                          master.getAnalog(ControllerAnalog::leftY), 
                          master.getAnalog(ControllerAnalog::rightX), 0.3);
    }
    pros::delay(20);
  }
}