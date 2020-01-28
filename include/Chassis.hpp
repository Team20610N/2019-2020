#include "main.h"
#pragma once

extern std::shared_ptr<ChassisController> chassis;
extern Motor frontLeftMotor;
extern Motor backLeftMotor;
extern Motor frontRightMotor;
extern Motor backRightMotor;

extern void ChassisOpcontrol(void* param);