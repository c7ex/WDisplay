#pragma once
#include"string"

HDC          hdc;
PAINTSTRUCT  ps;
RECT         rt;

// Timer
#define SENIOR_TIMER	0
#define INTERVAL_TIMER USER_TIMER_MINIMUM // (update form 10 ms)

// Mouse
double mouse_x;
double mouse_y;

// Palette
COLORREF CLR_BLACK = RGB(0x00, 0x00, 0x00);
COLORREF CLR_WHITE = RGB(0xFF, 0xFF, 0xFF);
COLORREF CLR_RED = RGB(0xFF, 0x00, 0x00);

// Object for paint
COLORREF TEXT_MOUSE_CURRENT_POSITION = CLR_WHITE;
HBRUSH BACKGROUND_BRUSH	= CreateSolidBrush(CLR_BLACK);
HPEN BACKGROUND_PEN		= CreatePen(PS_SOLID, 1, CLR_BLACK);
HPEN AXIS_PEN			= CreatePen(PS_SOLID, 1, CLR_WHITE);
HPEN TEST_OBJ_PEN		= CreatePen(PS_SOLID, 1, CLR_RED);

// Widgets for create form
// ...


// Data for Display
// ...