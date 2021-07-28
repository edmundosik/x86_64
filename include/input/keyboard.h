#pragma once
#include <stdint.h>
#include "input/kbScancodeTranslation.h"
#include "gfx/BasicRenderer.h"
#include "cstr.h"

void HandleKeyboard();
void clearBuffer(char buffer[]);

extern bool CanWrite;
extern bool CanErease;
extern char kbBuffer[100];
extern uint8_t kbScancode;