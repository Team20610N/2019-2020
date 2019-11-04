#include "main.h"
#pragma once

extern std::shared_ptr<ChassisController> chassis;
extern std::shared_ptr<okapi::HDriveModel> driveTrain;

void ChassisOpcontrol(void* param);