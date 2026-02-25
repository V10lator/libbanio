#pragma once

#include <stdint.h>

#define BANIO_MAX_DIRS 64

void banio_dirInit();
void banio_opendir(uint8_t *rdram, uint32_t payload);
void banio_closedir(uint8_t *rdram, uint32_t payload);
void banio_readdir(uint8_t *rdram, uint32_t payload);
