#include "modding.h"
#include "functions.h"
#include "variables.h"
#include "recomputils.h"
#include "recompconfig.h"

#include "banio_internal.h"
#include "libbanio.h"

int libbanio_errno;

RECOMP_EXPORT void libbanio_init()
{
    libbanio_api(LIBBANIO_CMD_INIT, 0);
}

RECOMP_EXPORT void libbanio_deinit()
{
    libbanio_api(LIBBANIO_CMD_DEINIT, 0);
}

RECOMP_EXPORT const char *libbanio_getVersion()
{
    static char payload[16] = { '\0' };
    libbanio_api(LIBBANIO_CMD_GET_VERSION, payload);
    return payload;
}

RECOMP_EXPORT int libbanio_getErrno()
{
    return libbanio_errno;
}
