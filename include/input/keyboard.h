#pragma once
#include <stdint.h>
#include "input/kbScancodeTranslation.h"
#include "gfx/BasicRenderer.h"
#include "cstr.h"
#include "memory/memory.h"
#include "string.h"

void HandleKeyboard();
char GetKey();
void clearBuffer(char buffer[]);

extern bool CanWrite;
extern bool CanErease;
extern char kbBuffer[100];
extern uint8_t kbScancode;