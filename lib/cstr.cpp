#include "cstr.h"

char uint_to_string[128];
const char* to_string(uint64_t value) {
    uint8_t size;
    uint64_t sizeTest = value;
    while(sizeTest / 10 > 0) {
        sizeTest /= 10;
        size++;
    }
    uint8_t index = 0;

    while(value / 10 > 0) {
        uint8_t remainder = value % 10;
        value /= 10;
        uint_to_string[size - index] = remainder + '0';
        index++;
    }
    uint8_t remainder = value % 10;
    uint_to_string[size - index] = remainder + '0';
    uint_to_string[size + 1] = 0;

    return uint_to_string;
}

char int_to_string[128];
const char* to_string(int64_t value) {
    uint8_t isNegative = 0;
    if(value < 0) {
        isNegative = 1;
        value *= -1;
        int_to_string[0] = '-';
    }

    uint8_t size;
    uint64_t sizeTest = value;
    while(sizeTest / 10 > 0) {
        sizeTest /= 10;
        size++;
    }
    uint8_t index = 0;

    while(value / 10 > 0) {
        uint8_t remainder = value % 10;
        value /= 10;
        int_to_string[isNegative + size - index] = remainder + '0';
        index++;
    }
    uint8_t remainder = value % 10;
    int_to_string[isNegative + size - index] = remainder + '0';
    int_to_string[isNegative + size + 1] = 0;
    
    return int_to_string;
}

char double_to_string[128];
const char* to_string(double value, uint8_t decimalCount) {
    if(decimalCount > 20) 
        decimalCount = 20;

    char* intPtr = (char*)to_string((int64_t)value);
    char* doublePtr = double_to_string;

    if(value < 0) 
        value *= -1;
        
    while(*intPtr != 0) {
        *doublePtr = *intPtr;
        intPtr++;
        doublePtr++;
    }
    
    *doublePtr = '.';
    doublePtr++;

    double newValue = value - (int)value;

    for (uint8_t i = 0; i < decimalCount; i++) {
        newValue *= 10;
        *doublePtr = (int)newValue + '0';
        newValue -= (int)newValue;
        doublePtr++;
    }
    
    *doublePtr = 0;
    return double_to_string;
}

const char* to_string(double value) {
    return to_string(value, 2);
}

char hex_to_string_out8[128];
char const* hex_to_string(uint8_t value) {
    uint8_t* valPtr = &value;
    uint8_t* ptr;
    uint8_t tmp;
    uint8_t size = 1 * 2 -1;
    for(uint8_t i = 0; i < size; i++) {
        ptr = ((uint8_t*)valPtr + i);
        tmp = ((*ptr & 0xF0) >> 4);
        hex_to_string_out8[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0');
        tmp = ((*ptr & 0x0F));
        hex_to_string_out8[size - (i * 2)] = tmp + (tmp > 9 ? 55 : '0');
    }

    hex_to_string_out8[size + 1] = 0;
    return hex_to_string_out8;
}

char hex_to_string_out16[128];
char const* hex_to_string(uint16_t value) {
    uint16_t* valPtr = &value;
    uint8_t* ptr;
    uint8_t tmp;
    uint8_t size = 2 * 2 -1;
    for(uint8_t i = 0; i < size; i++) {
        ptr = ((uint8_t*)valPtr + i);
        tmp = ((*ptr & 0xF0) >> 4);
        hex_to_string_out16[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0');
        tmp = ((*ptr & 0x0F));
        hex_to_string_out16[size - (i * 2)] = tmp + (tmp > 9 ? 55 : '0');
    }

    hex_to_string_out16[size + 1] = 0;
    return hex_to_string_out16;
}

char hex_to_string_out32[128];
char const* hex_to_string(uint32_t value) {
    uint32_t* valPtr = &value;
    uint8_t* ptr;
    uint8_t tmp;
    uint8_t size = 4 * 2 -1;
    for(uint8_t i = 0; i < size; i++) {
        ptr = ((uint8_t*)valPtr + i);
        tmp = ((*ptr & 0xF0) >> 4);
        hex_to_string_out32[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0');
        tmp = ((*ptr & 0x0F));
        hex_to_string_out32[size - (i * 2)] = tmp + (tmp > 9 ? 55 : '0');
    }

    hex_to_string_out32[size + 1] = 0;
    return hex_to_string_out32;
}

char hex_to_string_out[128];
char const* hex_to_string(uint64_t value) {
    uint64_t* valPtr = &value;
    uint8_t* ptr;
    uint8_t tmp;
    uint8_t size = 8 * 2 -1;
    for(uint8_t i = 0; i < size; i++) {
        ptr = ((uint8_t*)valPtr + i);
        tmp = ((*ptr & 0xF0) >> 4);
        hex_to_string_out[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0');
        tmp = ((*ptr & 0x0F));
        hex_to_string_out[size - (i * 2)] = tmp + (tmp > 9 ? 55 : '0');
    }

    hex_to_string_out[size + 1] = 0;
    return hex_to_string_out;
}

bool strcmp(char* a, char* b, uint64_t length) {
    for(int i = 0; i < length; i++) {
        //if(*a != *b)
        if(a[i] != b[i])
            return 0;
    }
    return 1;
}