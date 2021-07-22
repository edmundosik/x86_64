#include "input/mouse.h"

void MouseWait() {
    uint64_t timeout = 100000;
    while (timeout--){
        if((inb(0x64) & 0b10) == 0)
            return;
    }
}

void MouseWaitInput() {
    uint64_t timeout = 100000;
    while (timeout--){
        if(inb(0x64) & 0b1)
            return;
    }
}

void MouseWrite(uint8_t value) {
    MouseWait();
    outb(0x64, 0xD4);
    MouseWait();
    outb(0x60, value);
}

uint8_t MouseRead() {
    MouseWaitInput();
    return inb(0x60);
}

uint8_t mouseCycle = 0;
uint8_t mousePacket[4];
bool mousePacketReady = false;
Point MousePosition;

void HandlePS2Mouse(uint8_t data) {
    switch(mouseCycle){
        case 0:
            if(mousePacketReady)
                break;
            if(data & 0b00001000 == 0)
                break;
            mousePacket[0] = data;
            mouseCycle++;
            break;
        case 1:
            mousePacket[1] = data;
            mouseCycle++;
            break;
        case 2:
            mousePacket[2] = data;
            mousePacketReady = true;
            mouseCycle = 0;
            break;
    }
}

void InitPS2Mouse() {
    outb(0x64, 0xA8);
    MouseWait();
    outb(0x64, 0x20);
    MouseWaitInput();
    uint8_t status = inb(0x60);
    status |= 0b10;
    MouseWait();
    outb(0x64, 0x60);
    MouseWait();
    outb(0x60, status);

    MouseWrite(0xF6);
    MouseRead();

    MouseWrite(0xF4);
    MouseRead();
}

void ProccessMousePacket() {
    if(!mousePacketReady)
        return;
    mousePacketReady = false;

    bool xNegative, yNegative, xOverflow, yOverflow;
    if(mousePacket[0] & PS2XSign)
        xNegative = true;
    else
        xNegative = false;
    
    if(mousePacket[0] & PS2YSign)
        yNegative = true;
    else
        yNegative = false;

    if(mousePacket[0] & PS2XOverflow)
        xOverflow = true;
    else
        xOverflow = false;
    
    if(mousePacket[0] & PS2YOverflow)
        yOverflow = true;
    else
        yOverflow = false;

    if(!xNegative) {
        MousePosition.X += mousePacket[1];
        if(xOverflow) {
            MousePosition.X += 255;
        }
    } else {
        mousePacket[1] = 256 - mousePacket[1];
        MousePosition.X -= mousePacket[1];
        if(xOverflow) {
            MousePosition.X -= 255;
        }
    }

    if(!yNegative) {
        MousePosition.Y -= mousePacket[2];
        if(yOverflow) {
            MousePosition.Y -= 255;
        }
    } else {
        mousePacket[2] = 256 - mousePacket[2];
        MousePosition.Y += mousePacket[2];
        if(yOverflow) {
            MousePosition.Y += 255;
        }
    }

    if(MousePosition.X < 0)
        MousePosition.X = 0;
    if(MousePosition.X > renderer->TargetFrameBuffer->Width - 8)
        MousePosition.X = renderer->TargetFrameBuffer->Width - 8;

    if(MousePosition.Y < 0)
        MousePosition.Y = 0;
    if(MousePosition.Y > renderer->TargetFrameBuffer->Height - 16)
        MousePosition.Y = renderer->TargetFrameBuffer->Height - 16;

    renderer->putChar('\'', MousePosition.X, MousePosition.Y);
    mousePacketReady = false;
}