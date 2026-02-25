#include "modding.h"
#include "functions.h"
#include "variables.h"
#include "recomputils.h"
#include "recompconfig.h"

#include "banio_internal.h"
#include "libbanio.h"

RECOMP_EXPORT FILE *libbanio_fopen(const char *name, const char *mode)
{
    char payload[1024];
    size_t s = strlen((char *)name) + 1;
    memcpy(payload, (void *)name, s);
    strcpy(payload + s, (char *)mode);

    libbanio_api(LIBBANIO_CMD_FOPEN, payload);

    libbanio_errno = *(s32 *)(payload + sizeof(FILE *));
    return (FILE *)*payload;
}

RECOMP_EXPORT int libbanio_fclose(FILE *file)
{
    u32 payload[2] = { (u32)file, 0 };

    libbanio_api(LIBBANIO_CMD_FCLOSE, payload);

    libbanio_errno = payload[1];
    return (int)payload[0];
}

RECOMP_EXPORT u32 libbanio_fread(void *buf, u32 size, u32 n, FILE *file)
{
    char payload[1024];
    memcpy(payload, &buf, sizeof(void *));
    memcpy(payload + sizeof(void *), &size, sizeof(u32));
    memcpy(payload + sizeof(void *) + sizeof(u32), &n, sizeof(u32));
    memcpy(payload + sizeof(void *) + (sizeof(u32) * 2), &file, sizeof(FILE *));

    libbanio_api(LIBBANIO_CMD_FREAD, payload);

    libbanio_errno = *(s32 *)(payload + sizeof(s32));
    return *(s32 *)payload;
}

RECOMP_EXPORT u32 libbanio_fwrite(void *buf, u32 size, u32 n, FILE *file)
{
    char payload[1024];
    memcpy(payload, &buf, sizeof(void *));
    memcpy(payload + sizeof(void *), &size, sizeof(u32));
    memcpy(payload + sizeof(void *) + sizeof(u32), &n, sizeof(u32));
    memcpy(payload + sizeof(void *) + (sizeof(u32) * 2), &file, sizeof(FILE *));

    libbanio_api(LIBBANIO_CMD_FWRITE, payload);

    libbanio_errno = *(s32 *)(payload + sizeof(s32));
    return *(s32 *)payload;
}

RECOMP_EXPORT s32 libbanio_fseek(FILE *file, u32 v, s32 w)
{
    u32 payload[3] = { (u32)file, v, (u32)w };

    libbanio_api(LIBBANIO_CMD_FSEEK, payload);

    libbanio_errno = payload[1];
    return payload[0];
}

RECOMP_EXPORT u32 libbanio_ftell(FILE *file)
{
    u32 payload[2] = { (u32)file, 0 };

    libbanio_api(LIBBANIO_CMD_FTELL, payload);

    libbanio_errno = payload[1];
    return payload[0];
}
