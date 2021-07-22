#include "interrupts/interrupts.h"

__attribute__((interrupt)) void PageFault_Handler(struct interrupt_frame* frame) {
    panic("PAGE_FAULT");

    while(true);
}