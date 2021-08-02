#include "date.h"

uint64_t counter = 0;
uint64_t segBuffer[16 * 8];
Point counterPosition;

void updateCounter() {
    if(counter > 9) counter = 0;
    counterPosition = {(uint16_t)renderer->TargetFrameBuffer->Width - 8, 0};
    PIT::Sleep(1000);
    renderer->rect(counterPosition, {8, 16}, 0xff4B5263);
    renderer->print(to_string(counter), counterPosition);
    counter++;
}