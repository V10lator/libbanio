#include "modding.h"
#include "functions.h"
#include "variables.h"
#include "recomputils.h"
#include "recompconfig.h"

#include "banio_internal.h"
#include "libbanio.h"

#define BANIO_MAX_DIRS 64 // TODO

static banio_dirent rets[BANIO_MAX_DIRS];

RECOMP_EXPORT DIR *libbanio_opendir(const char *name)
{
    char payload[1024];
    strcpy(payload, (char *)name);

    libbanio_api(LIBBANIO_CMD_OPENDIR, payload);

    libbanio_errno = *(s32 *)(payload + sizeof(DIR *));
    return (DIR *)*payload;
}

RECOMP_EXPORT int libbanio_closedir(DIR *dir)
{
    u32 payload[2] = { (u32)dir, 0 };

    libbanio_api(LIBBANIO_CMD_CLOSEDIR, payload);

    libbanio_errno = payload[1];
    return (int)payload[0];
}

RECOMP_EXPORT struct banio_dirent *libbanio_readdir(DIR *dir)
{
    char payload[1024];
    memcpy(payload, &dir, sizeof(DIR *));

    libbanio_api(LIBBANIO_CMD_READDIR, payload);
    if(!payload[0])
        return NULL;

    banio_dirent *ret = rets + ((u32)dir) - 1;
    ret->d_type = (unsigned char)(payload[1]);
    strcpy(ret->d_name, payload + 2);
    return ret;
}
