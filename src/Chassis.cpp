#include "main.h"
// #include "fstream"

ADIEncoder leftEncoder('E', 'F');
ADIEncoder rightEncoder('A', 'B');
ADIEncoder middleEncoder('C', 'D');
ADIGyro Gyro('H',1);


// chassis declaration
std::shared_ptr<ChassisController> chassis = ChassisControllerBuilder()
  .withMotors(5, -4, 3)
  .withSensors(leftEncoder, rightEncoder)
  .withGearset(AbstractMotor::gearset::green)
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
  Motor leftMotor(5, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
  Motor rightMotor(4, true, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
  Motor middleMotor(3, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
  int leftMotorControl = 0;
  int rightMotorControl = 0;
  int middleMotorControl = 0;
  int forward, straff, turning;
  int temp;
  float theta;
  bool FieldCenteric = true;
  
  ControllerButton fieldCentericToggle(ControllerDigital::A);
  ControllerButton gyroReset(ControllerDigital::X);

  while (isAuton == false) {
    chassisHeading = Gyro.get() / 10;
    newHeading = chassisHeading - JoisticHeading;
    
    //Updates display values.
		// updateLineVariable(1, chassisHeading);
		// updateLineVariable(2, JoisticHeading); //middleEncoder.controllerGet()
		// updateLineVariable(3, leftEncoder.controllerGet());
		// updateLineVariable(4, rightEncoder.controllerGet());
    
    // Graphs the drive motor temps
    lv_chart_set_next(chart, GreenLine, leftMotor.getTemperature());
    lv_chart_set_next(chart, LimeLine, rightMotor.getTemperature());
    lv_chart_set_next(chart, SilverLine, middleMotor.getTemperature());
    
    
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
    updateLineVariable(1, theta);
		updateLineVariable(2, temp);
		updateLineVariable(3, straff);
		updateLineVariable(4, forward);
    
    driveTrain->hArcade(straff, 
                        forward, 
                        turning, 0);

    pros::delay(20);
  }
}