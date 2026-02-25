#pragma once

#include <stdint.h>

int strlen_n64(uint8_t *rdram, uint32_t ptr);
void strcpy_to_n64(uint8_t *rdram, uint32_t to, const char* from);
void memcpy_from_n64(uint8_t *rdram, uint8_t *to, uint32_t from, int size);
void memcpy_to_n64(uint8_t *rdram, uint32_t to, uint8_t *from, int size);
