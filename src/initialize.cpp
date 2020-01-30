#include "main.h"

void initialize() {
	startingAnglerAngle = AnglerAngle.get();
	pros::delay(2000);
	runScreen();
}

void disabled() {}

void competition_initialize() {}