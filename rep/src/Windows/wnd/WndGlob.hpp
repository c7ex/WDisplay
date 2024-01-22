#pragma once
#include"windows.h"
#include"string"

// Timer
#define ID_SENIOR_TIMER	0
#define INTERVAL_TIMER USER_TIMER_MINIMUM // (update form 10 ms)

// Widgets for create form

	// mouse position
	int xPos;
	int yPos;

	// hold click left button mouse
	bool LBUTTON_MOUSE_ACTIVE;

	int buttondown_save_x_pos;
	int buttondown_save_y_pos;

	int memory_buttondown_save_x_pos;
	int memory_buttondown_save_y_pos;

	int shift_x;
	int shift_y;
	double scale;

// Data for Display