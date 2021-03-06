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

Motor frontLeftMotor(9, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor backLeftMotor(10, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor frontRightMotor(2, true, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor backRightMotor(3, true, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
std::shared_ptr<OdomChassisController> chassis = ChassisControllerBuilder()
	.withMotors(frontLeftMotor, frontRightMotor, backRightMotor, backLeftMotor)
  .withMaxVelocity(150)
  .withSensors(leftEncoder, rightEncoder)
    .withGains(
        {0.0035, 0, 0}, // Distance controller gains
        {0.003, 0, 0}, // Turn controller gains
        {0.002, 0, 0.00006}  // Angle controller gains (helps drive straight)
    )
    .withDimensions(AbstractMotor::gearset::green, {{3.5_in, 23.5_in}, imev5BlueTPR})
    .withOdometry() // use the same scales as the chassis (above)
    .buildOdometry(); // build an odometry chassis

std::shared_ptr<okapi::XDriveModel> driveTrain = std::dynamic_pointer_cast<XDriveModel>(chassis->getModel());

// Thread that controls the chassis 
void ChassisOpcontrol(void* param) {
  chassis->moveDistance(0_in);
  // assigning the chassis to a X-drive model
  driveTrain->setBrakeMode(AbstractMotor::brakeMode::hold);
  
  int leftMotorControl = 0;
  int rightMotorControl = 0;
  int middleMotorControl = 0;
  double forward, straff, turning;
  int temp;
  float theta;
  bool FieldCenteric = true;
  
  ControllerButton fieldCentericToggle(ControllerDigital::X);
  ControllerButton gyroReset(ControllerDigital::Y);

  while (isAuton == false) {
    chassisHeading = IMU.get_heading() -headingError;
    newHeading = chassisHeading - JoisticHeading;

    //Updates display values.
		updateLineVariable(1, chassisHeading);
		updateLineVariable(3, leftEncoder.get());
		updateLineVariable(4, rightEncoder.get());
    
    // Graphs the drive motor temps
    lv_chart_set_next(chart, NavyLine, frontLeftMotor.getTemperature());
    lv_chart_set_next(chart, BlueLine, frontRightMotor.getTemperature());
    lv_chart_set_next(chart, GreenLine, backLeftMotor.getTemperature());
    lv_chart_set_next(chart, LimeLine, backRightMotor.getTemperature());
    
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
        master.setText(1, 8, "False");
        FieldCenteric = false;
      }
      else {
        master.setText(1, 8, "True");
        FieldCenteric = true;
      }
    }
    
    if (gyroReset.isPressed()) {
      headingError = IMU.get_heading();
    }
    
    std::cout << IMU.get_pitch() << std::endl;
    if (IMU.get_pitch() < -10) {
      driveTrain->forward(-30);
      pros::delay(200);
    }
    else {
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
    }
    pros::delay(20);
  }
}