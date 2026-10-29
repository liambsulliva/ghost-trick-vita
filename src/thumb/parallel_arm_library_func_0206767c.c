extern char * func_02008444(void);
extern void func_020297f0(char *, int);

void func_0206767c(unsigned char *object)
{
    int slot;

    *(unsigned short *)(object + 0xd6) = 0;
    *(unsigned short *)(object + 0xd8) = 0x3c;
    object[0xde] = 1;
    slot = *(int *)(object + 0x84);
    if (slot >= 0) {
        func_020297f0(func_02008444(), slot);
        *(int *)(object + 0x84) = -1;
    }
}
