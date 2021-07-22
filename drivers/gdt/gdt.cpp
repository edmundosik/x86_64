#include "gdt/gdt.h"

//__attribute__((aligned(0x1000)));
GDT DefaultGDT = {
    {0, 0, 0, 0x00, 0x00, 0}, // NULL
    {0, 0, 0, 0x9A, 0xA0, 0}, // Kernel Code Segment
    {0, 0, 0, 0x92, 0xA0, 0}, // Kernel Data Segment
    {0, 0, 0, 0x00, 0x00, 0}, // User NULL
    {0, 0, 0, 0x9A, 0xA0, 0}, // User Code Segment
    {0, 0, 0, 0x92, 0xA0, 0} // User Data Segment
};
