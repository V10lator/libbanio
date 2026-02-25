#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "banio_internal.h"
#include "dir.h"
#include "file.h"
#include "n64.h"
#include "recomp_dll.h"

DLLEXPORT uint32_t recomp_api_version = 1;
static unsigned int refcount = 0;

static void banio_init()
{
    if(!refcount++)
    {
        banio_fileInit();
        banio_dirInit();
    }
}

static void banio_deinit()
{
    if(!refcount)
        return;

    refcount--;
}

static void banio_getVersion(uint8_t *rdram, uint32_t payload)
{
    strcpy_to_n64(rdram, payload, "0.0.1");
}

DLLEXPORT void libbanio_api(uint8_t* rdram, recomp_context* ctx)
{
    uint64_t *args = &ctx->r4;
    LIBBANIO_CMD cmd = (uint32_t)args[0];

    switch(cmd)
    {
        // No category
        case LIBBANIO_CMD_INIT:
            banio_init();
            break;
        case LIBBANIO_CMD_DEINIT:
            banio_deinit();
            break;
        case LIBBANIO_CMD_GET_VERSION:
            banio_getVersion(rdram, args[1]);
            break;

        // File I/O
        case LIBBANIO_CMD_FOPEN:
            banio_fopen(rdram, args[1]);
            break;
        case LIBBANIO_CMD_FCLOSE:
            banio_fclose(rdram, args[1]);
            break;
        case LIBBANIO_CMD_FREAD:
            banio_fread(rdram, args[1]);
            break;
        case LIBBANIO_CMD_FWRITE:
            banio_fwrite(rdram, args[1]);
            break;
        case LIBBANIO_CMD_FSEEK:
            banio_fseek(rdram, args[1]);
            break;
        case LIBBANIO_CMD_FTELL:
            banio_ftell(rdram, args[1]);
            break;
        case LIBBANIO_CMD_UNLINK:
            banio_unlink(rdram, args[1]);
            break;

        // Dir I/O
        case LIBBANIO_CMD_OPENDIR:
            banio_opendir(rdram, args[1]);
            break;
        case LIBBANIO_CMD_CLOSEDIR:
            banio_closedir(rdram, args[1]);
            break;
        case LIBBANIO_CMD_READDIR:
            banio_readdir(rdram, args[1]);
            break;

    }

    RECOMP_RETURN(0);
}
