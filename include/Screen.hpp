#include "main.h"
#pragma once

extern lv_obj_t * chart;

extern lv_obj_t * tabview;

extern lv_chart_series_t * redLine;

extern lv_chart_series_t * NavyLine;
extern lv_chart_series_t * BlueLine;

extern lv_chart_series_t * GreenLine;
extern lv_chart_series_t * LimeLine;

extern lv_chart_series_t * SilverLine;
extern lv_chart_series_t * WhiteLine;

extern lv_chart_series_t * PurpleLine;
extern lv_chart_series_t * OrangeLine;

extern int selectedAuton;
extern bool test1;
extern bool test2;
extern bool test3;
extern void SetAutonButton(int slot);

void runScreen();
void updateLineVariable(int line, double value);
void updateLineVariable(int line, std::string value);
void startAuton(void*);
