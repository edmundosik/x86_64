#pragma once
#include "io.h"
#include "math.h"
#include "gfx/BasicRenderer.h"

#define PS2XSign 0b00010000
#define PS2YSign 0b00100000
#define PS2XOverflow 0b01000000
#define PS2YOverflow 0b10000000

#define PS2LeftButton 0b00000001
#define PS2MiddleButton 0b00000100
#define PS2RightButton 0b00000010

extern uint8_t mousePointer[];

void HandlePS2Mouse(uint8_t data);
void InitPS2Mouse();
void ProccessMousePacket();

extern Point MousePosition;