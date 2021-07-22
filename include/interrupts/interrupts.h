#pragma once
#include "BasicRenderer.h"
#include "panic.h"

struct interrupt_frame;

__attribute__((interrupt)) void PageFault_Handler(struct interrupt_frame* frame);