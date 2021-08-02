#pragma once
#include <stdint.h>
#include "io.h"

// Ports
#define COM1 0x3F8
#define COM2 0x2F8
#define COM3 0x3E8
#define COM4 0x2E8

int InitSerial();
int RecievedSerial();
char SerialRead();
int IsTransmitEmpty();
void SerialWrite(char data);


