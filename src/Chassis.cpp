#include "main.h"

ADIEncoder leftEncoder('E', 'F');
ADIEncoder rightEncoder('C', 'D');
ADIGyro Gyro('B',1);

int newHeading = 0;
int JoisticHeading = 0;
int chassisHeading;
bool chassisBrake = false;

Motor frontLeftMotor(9, false, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);
Motor backLeftMotor(10, false, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);
Motor frontRightMotor(2, true, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);
Motor backRightMotor(1, true, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);
std::shared_ptr<ChassisController> chassis = ChassisControllerBuilder()
  .withMotors(frontLeftMotor, frontRightMotor, backLeftMotor, backRightMotor)
  // .withSensors(leftEncoder, rightEncoder)
  .withDimensions(AbstractMotor::gearset::red, {{4_in, 10_in}, imev5RedTPR})
  .withMaxVelocity(60)
  .withLogger(
      std::make_shared<Logger>(
          TimeUtilFactory::createDefault().getTimer(), // It needs a Timer
          "/usd/Chassis_Diagnostics", // Output to the SDCard
          Logger::LogLevel::info // Most verbose log level
      )
  )
  .build();

// Thread that controls the chassis 
void ChassisOpcontrol(void* param) {
  // // Write GyroHeading and ChassisVelocity to a file
  // FILE *fp = fopen("/usd/Chassis_Diagnostics", "a");
  // std::string diagnosticsStr = "Testing\n";
  // fprintf(fp, "%s", diagnosticsStr.c_str()); 
  // fclose(fp);
    
  // assigning the chassis to a H-drive model
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
    chassisHeading = Gyro.get() / 10;
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
    }
    
    if (FieldCenteric == true) {
      driveTrain->xArcade((turning / 100),
                          (forward / 100),
                          (straff / 100), 0);
    }
    else {
      driveTrain->xArcade(master.getAnalog(ControllerAnalog::rightX), 
                          master.getAnalog(ControllerAnalog::leftY), 
                          master.getAnalog(ControllerAnalog::leftX), 0.3);
    }
    pros::delay(20);
  }
}