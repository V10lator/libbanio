#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "recomp_dll.h"

#include <file.h>
#include <n64.h>

static FILE *openFiles[BANIO_MAX_FILES];

void banio_fileInit()
{
    for(int i = 0; i < BANIO_MAX_FILES; i++)
        openFiles[i] = NULL;
}

void banio_fopen(uint8_t *rdram, uint32_t payload)
{
    uint32_t i = 0;
    for( ; i < BANIO_MAX_FILES; i++)
        if(openFiles[i] == NULL)
            break;

    uint32_t e;

    if(i != BANIO_MAX_FILES)
    {
        int l = strlen_n64(rdram, payload);
        char path[l];
        memcpy_from_n64(rdram, (uint8_t *)path, payload, l);

        int l2 = strlen_n64(rdram, payload + l);
        char mode[l2];
        memcpy_from_n64(rdram, (uint8_t *)mode, payload + l, l2);

        openFiles[i++] = fopen(path, mode);
        e = errno;
    }
    else
    {
        i = 0;
        e = 1; // TODO
    }

    memcpy_to_n64(rdram, payload, (uint8_t *)&i, sizeof(uint32_t));
    memcpy_to_n64(rdram, payload + sizeof(uint32_t), (uint8_t *)&e, sizeof(int32_t));
}

void banio_fclose(uint8_t *rdram, uint32_t payload)
{
    uint32_t i;
    memcpy_from_n64(rdram, (uint8_t *)&i, payload, sizeof(uint32_t));
    i = bswap_32(i);

    int32_t ret = fclose(openFiles[--i]);
    int32_t e = errno;
    openFiles[i] = NULL;

    memcpy_to_n64(rdram, payload, (uint8_t *)&ret, sizeof(int32_t));
    memcpy_to_n64(rdram, payload + sizeof(int32_t), (uint8_t *)&e, sizeof(int32_t));
}

void banio_fread(uint8_t *rdram, uint32_t payload)
{
    uint32_t buf;
    memcpy_from_n64(rdram, (uint8_t *)&buf, payload, sizeof(uint32_t));
    buf = bswap_32(buf);

    uint32_t size;
    memcpy_from_n64(rdram, (uint8_t *)&size, payload + sizeof(uint32_t), sizeof(uint32_t));
    size = bswap_32(size);

    uint32_t n;
    memcpy_from_n64(rdram, (uint8_t *)&n, payload + (sizeof(uint32_t) * 2), sizeof(uint32_t));
    n = bswap_32(n);

    uint32_t i;
    memcpy_from_n64(rdram, (uint8_t *)&i, payload + (sizeof(uint32_t) * 3), sizeof(uint32_t));
    i = bswap_32(i);

    uint8_t buf2[size * n];
    uint32_t ret = fread(buf2, size, n, openFiles[--i]);
    int32_t e = errno;

    if(ret == n)
        memcpy_to_n64(rdram, buf, buf2, size * n);

    ret = bswap_32(ret);
    memcpy_to_n64(rdram, payload, (uint8_t *)&ret, sizeof(uint32_t));
    e = bswap_32(e);
    memcpy_to_n64(rdram, payload + sizeof(uint32_t), (uint8_t *)&e, sizeof(int32_t));
}

void banio_fwrite(uint8_t *rdram, uint64_t payload)
{
    uint32_t buf;
    memcpy_from_n64(rdram, (uint8_t *)&buf, payload, sizeof(uint32_t));
    buf = bswap_32(buf);

    uint32_t size;
    memcpy_from_n64(rdram, (uint8_t *)&size, payload + sizeof(uint32_t), sizeof(uint32_t));
    size = bswap_32(size);

    uint32_t n;
    memcpy_from_n64(rdram, (uint8_t *)&n, payload + (sizeof(uint32_t) * 2), sizeof(uint32_t));
    n = bswap_32(n);

    uint32_t i;
    memcpy_from_n64(rdram, (uint8_t *)&i, payload + (sizeof(uint32_t) * 3), sizeof(uint32_t));
    i = bswap_32(i);

    uint32_t ret = size * n;
    uint8_t buf2[ret];
    memcpy_from_n64(rdram, buf2, buf, ret);
    ret = fwrite(buf2, size, n, openFiles[--i]);
    int32_t e = errno;

    ret = bswap_32(ret);
    memcpy_to_n64(rdram, payload, (uint8_t *)&ret, sizeof(uint32_t));
    e = bswap_32(e);
    memcpy_to_n64(rdram, payload + sizeof(uint32_t), (uint8_t *)&e, sizeof(int32_t));
}

void banio_fseek(uint8_t *rdram, uint64_t payload)
{
    uint32_t i;
    memcpy_from_n64(rdram, (uint8_t *)&i, payload, sizeof(uint32_t));
    i = bswap_32(i);

    uint32_t v;
    memcpy_from_n64(rdram, (uint8_t *)&v, payload + sizeof(uint32_t), sizeof(uint32_t));
    v = bswap_32(v);

    int32_t w;
    memcpy_from_n64(rdram, (uint8_t *)&w, payload + (sizeof(uint32_t) * 2), sizeof(uint32_t));
    w = bswap_32(w);

    v = fseek(openFiles[--i], v, w);
    w = errno;
    if(v != 0)
        v = (uint32_t)-1;

    v = bswap_32(v);
    memcpy_to_n64(rdram, payload, (uint8_t *)&v, sizeof(uint32_t));
    w = bswap_32(w);
    memcpy_to_n64(rdram, payload + sizeof(uint32_t), (uint8_t *)&w, sizeof(int32_t));
}

void banio_ftell(uint8_t *rdram, uint64_t payload)
{
    int32_t i;
    memcpy_from_n64(rdram, (uint8_t *)&i, payload, sizeof(int32_t));
    i = bswap_32(i);

    i = ftell(openFiles[--i]);
    int32_t e = errno;

    i = bswap_32(i);
    memcpy_to_n64(rdram, payload, (uint8_t *)&i, sizeof(uint32_t));
    e = bswap_32(e);
    memcpy_to_n64(rdram, payload + sizeof(uint32_t), (uint8_t *)&e, sizeof(int32_t));
}

void banio_unlink(uint8_t *rdram, uint64_t payload)
{
    int l = strlen_n64(rdram, payload);
    char path[l];
    memcpy_from_n64(rdram, (uint8_t *)path, payload, l);

    int32_t ret = unlink(path);
    int32_t e = errno;

    ret = bswap_32(ret);
    memcpy_to_n64(rdram, payload, (uint8_t *)&ret, sizeof(int32_t));
    e = bswap_32(e);
    memcpy_to_n64(rdram, payload + sizeof(int32_t), (uint8_t *)&e, sizeof(int32_t));
}
