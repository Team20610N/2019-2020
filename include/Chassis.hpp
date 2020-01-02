#include "main.h"
#pragma once

extern Motor leftMotor;
extern Motor rightMotor;
extern Motor middleMotor;
extern std::shared_ptr<ChassisController> chassis;

extern void ChassisOpcontrol(void* param);