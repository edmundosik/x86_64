#include "string.h"

uint64_t strlen(char* str) {
    int out = 0;
    for(int i = 0; str[i] != 0; i++){
        out++;
    }
    return out;
}

bool strcmp(char* a, char* b) {
    for(int i = 0; i < strlen(a); i++) {
        //if(*a != *b)
        if(a[i] != b[i])
            return 0;
    }
    return 1;
}