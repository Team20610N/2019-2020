#include "main.h"

void initialize() {
	startingAnglerAngle = AnglerAngle.get();
	pros::delay(2000);
	runScreen();
}

void disabled() {
	lv_tabview_set_tab_act(tabview, 1, true);
}

void competition_initialize() {}