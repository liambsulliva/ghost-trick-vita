extern char *func_02000d9c(char *object, char *param, unsigned int value);
extern void func_02006954(char *a, unsigned int b, unsigned int c, int d, unsigned int e);
extern void func_02006358(char *a, int b);
int func_02006444(char *param_1, int param_2, unsigned int param_3)
{
    char *top;
    char *obj;
    char *record;
    char *base;
    char *slot;
    unsigned int flags;
    int offset;
    top = (char *)0x020b18a0;
    obj = *(char **)(*(char **)(top + 0x10) + 0x1580);
    record = func_02000d9c(obj, param_1, *(unsigned int *)(*(char **)(obj + 0x98) + 0x24));
    if (record == 0) {
        return -5;
    }
    base = *(char **)(top + 0x10) + 0x3d28;
    offset = *(unsigned char *)(record + 6) * 0x3a4;
    flags = *(unsigned int *)(base + offset + 0xe8);
    if ((flags & 1) == 0) {
        return -6;
    }
    slot = base + offset + 0x70;
    if (slot == 0) {
        return -5;
    }
    *(unsigned int *)(base + offset + 0xe8) = *(unsigned int *)(base + offset + 0xe8) & ~4u;
    *(int *)(slot + 0x14) = 3;
    *(unsigned char *)(slot + 0xd) = 8;
    if (param_3 != 0) {
        func_02006954(slot, *(unsigned char *)(base + offset + 0x60), (unsigned char)param_2, 2, param_3);
        *(unsigned int *)(base + offset + 0xe8) |= 4;
        *(unsigned char *)(slot + 0xd) = 7;
    } else {
        func_02006358(param_1, param_2);
    }
    return 0;
}
