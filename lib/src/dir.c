#include <dirent.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include <stdio.h>

#include "recomp_dll.h"

#include <dir.h>
#include <n64.h>

static DIR *openDirs[BANIO_MAX_DIRS];


void banio_dirInit()
{
    for(int i = 0; i < BANIO_MAX_DIRS; i++)
        openDirs[i] = NULL;
}

void banio_opendir(uint8_t *rdram, uint32_t payload)
{
    uint32_t i = 0;
    for( ; i < BANIO_MAX_DIRS; i++)
        if(openDirs[i] == NULL)
            break;

    uint32_t e;

    if(i != BANIO_MAX_DIRS)
    {
        int l = strlen_n64(rdram, payload);
        char path[l];
        memcpy_from_n64(rdram, (uint8_t *)path, payload, l);

        openDirs[i++] = opendir(path);
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

void banio_closedir(uint8_t *rdram, uint32_t payload)
{
    uint32_t i;
    memcpy_from_n64(rdram, (uint8_t *)&i, payload, sizeof(uint32_t));
    i = bswap_32(i);

    int32_t ret = closedir(openDirs[--i]);
    int32_t e = errno;
    openDirs[i] = NULL;

    memcpy_to_n64(rdram, payload, (uint8_t *)&ret, sizeof(int32_t));
    memcpy_to_n64(rdram, payload + sizeof(int32_t), (uint8_t *)&e, sizeof(int32_t));
}

void banio_readdir(uint8_t *rdram, uint32_t payload)
{
    uint32_t i;
    memcpy_from_n64(rdram, (uint8_t *)&i, payload, sizeof(uint32_t));
    i = bswap_32(i);

    struct dirent *entry = readdir(openDirs[--i]);
    uint8_t success = entry != NULL;

    memcpy_to_n64(rdram, payload, &success, sizeof(uint8_t));
    if(success)
    {
        unsigned char type =
#if defined(_DIRENT_HAVE_D_TYPE)
        entry->d_type;
#else
        0; // TODO
#endif

        memcpy_to_n64(rdram, ++payload, &type, sizeof(unsigned char));
        strcpy_to_n64(rdram, ++payload, entry->d_name);
    }
}
