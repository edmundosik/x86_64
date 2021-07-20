#pragma once
#include <stdint.h>

const char* to_string(uint64_t value);
const char* to_string(int64_t value);

const char* to_string(double value, uint8_t decimalCount);
const char* to_string(double value);

const char* hex_to_string(uint8_t value);
const char* hex_to_string(uint16_t value);
const char* hex_to_string(uint32_t value);
const char* hex_to_string(uint64_t value);