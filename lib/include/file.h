#pragma once

#include <stdint.h>

#define BANIO_MAX_FILES 64

void banio_fileInit();
void banio_fopen(uint8_t *rdram, uint32_t payload);
void banio_fclose(uint8_t *rdram, uint32_t payload);
void banio_fread(uint8_t *rdram, uint32_t payload);
void banio_fwrite(uint8_t *rdram, uint64_t payload);
void banio_fseek(uint8_t *rdram, uint64_t payload);
void banio_ftell(uint8_t *rdram, uint64_t payload);
void banio_unlink(uint8_t *rdram, uint64_t payload);
