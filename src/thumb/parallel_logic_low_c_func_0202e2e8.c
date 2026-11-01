int func_020790c8();

int func_0202e2e8(unsigned char *obj, int arg)
{
    short i;
    i = 0;
    while ((unsigned int)i < *(unsigned int *)(obj + 0x158)) {
        if (func_020790c8(*(int *)(obj + 0x160) + ((int *)*(int *)(obj + 0x15c))[i], arg) == 0) {
            return i;
        }
        i = i + 1;
    }
    return -1;
}
