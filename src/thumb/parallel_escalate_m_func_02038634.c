extern int func_02039ea0(int object);
extern void func_0203952c(int object);

int func_02038634(int *object)
{
    int found;

    if (*(unsigned short *)((char *)object + 0x26) != 0) {
        *(unsigned short *)((char *)object + 0x26) = *(unsigned short *)((char *)object + 0x26) - 1;
        return 0;
    }
    if (*((unsigned char *)object + 0x34) != 0) {
        found = func_02039ea0(*object);
        if (found != 0) {
            func_0203952c(found);
        }
        *((unsigned char *)object + 0x34) = 0;
        *(unsigned short *)((char *)object + 0x26) = 0x78;
        return 0;
    }
    return 1;
}
