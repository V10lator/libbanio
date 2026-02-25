#include <stdint.h>

int strlen_n64(uint8_t *rdram, uint32_t ptr)
{
    ptr -= 0x80000000;

    for(int i = 1; 1; i++)
    {
        if(rdram[ptr++ ^ 3] == '\0')
            return i;
    }

    return -1;
}

void strcpy_to_n64(uint8_t *rdram, uint32_t to, const char* from)
{
    to -= 0x80000000;

    while(1)
    {
        rdram[to ^ 3] = *from;
        if(*from == '\0')
            return;

        from++;
        to++;
    }
}

void memcpy_from_n64(uint8_t *rdram, uint8_t *to, uint32_t from, int size)
{
    from -= 0x80000000;

    for(int i = 0; i < size; i++)
        *to++ = rdram[from++ ^ 3];
}

void memcpy_to_n64(uint8_t *rdram, uint32_t to, uint8_t *from, int size)
{
    to -= 0x80000000;

    for(int i = 0; i < size; i++)
        rdram[to++ ^ 3] = *from++;
}
