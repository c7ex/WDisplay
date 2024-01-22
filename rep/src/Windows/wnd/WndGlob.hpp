#pragma once
#include"windows.h"
#include"string"

// Timer
#define ID_SENIOR_TIMER	0
#define INTERVAL_TIMER USER_TIMER_MINIMUM // (update form 10 ms)

// Mouse events

	// mouse position
	int xPos;
	int yPos;

	// hold click left button mouse
	bool LBUTTON_MOUSE_ACTIVE;
	int shift_x;
	int shift_y;
	int buttondown_save_x_pos;
	int buttondown_save_y_pos;
	int memory_buttondown_save_x_pos;
	int memory_buttondown_save_y_pos;
	
	// scale
	double scale = 1;
	double scale_rate = 1;


// Object for paint

HPEN AXIS_PEN		= CreatePen(PS_SOLID, 1, RGB(0x0F, 0x7F, 0x7F));
HPEN TEST_OBJ_PEN	= CreatePen(PS_SOLID, 1, RGB(0x7F, 0x7F, 0x0F));


// Widgets for create form
// ...


// Data for Display
// ...