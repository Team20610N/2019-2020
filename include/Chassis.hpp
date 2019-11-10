#include "main.h"
#pragma once

// Globiling the drive train objects
extern std::shared_ptr<ChassisController> chassis;
extern std::shared_ptr<okapi::HDriveModel> driveTrain;

extern void ChassisOpcontrol(void* param);