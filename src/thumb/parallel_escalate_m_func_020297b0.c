extern void *func_02008444(void);
extern void func_020297f0(void *object, int handle);

void func_020297b0(char *object)
{
    int handle;

    *(unsigned short *)(object + 0x4c) = 0;
    *(unsigned short *)(object + 0x4e) = 0x3c;
    *(unsigned char *)(object + 0x54) = 1;
    *(unsigned char *)(object + 0x57) = 0;
    handle = *(int *)(object + 0x5c);
    *(unsigned short *)(object + 0x50) = 0;
    if (handle >= 0) {
        func_020297f0(func_02008444(), handle);
        *(int *)(object + 0x5c) = -1;
    }
}
