#pragma once

RECOMP_IMPORT(".", int libbanio_api(u32 cmd, void *payload));

typedef struct banio_dirent {
    unsigned char d_type;
    char d_name[256];
} banio_dirent;

typedef u32 FILE;
typedef u32 DIR;

extern int libbanio_errno;
