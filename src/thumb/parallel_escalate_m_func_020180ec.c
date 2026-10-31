extern int func_02018d3c(unsigned char *object);
extern unsigned int func_02018dd4(unsigned char *object);
extern int func_02018e0c(unsigned char *object, unsigned int flags);
extern int func_02019358(unsigned char *object);
extern void func_02017664(unsigned char *object);
extern void func_02018c6c(unsigned char *object);
extern void func_02018e9c(unsigned char *object);

int func_020180ec(unsigned char *object)
{
    unsigned int flags;
    unsigned short *cursor;

    if (func_02018d3c(object) == 1) {
        return 1;
    }
    flags = func_02018dd4(object);
    if (func_02018e0c(object, flags) == 1) {
        return 1;
    }
    if (object[0x8f] != 0) {
        if (func_02019358(object) != 0) {
            return 1;
        }
        cursor = *(unsigned short **)(object + 0x14);
        if (*cursor != 0xfffe) {
            func_02017664(object);
            if (*cursor == 0xff15) {
                *(int *)(object + 0x14) += 2;
            }
            func_02018c6c(object);
            *(int *)(object + 0x14) += 2;
            *(int *)(object + 0x7c) = *(int *)(object + 0x14);
            object[0x8f] = 0;
        } else {
            object[0x59] = 1;
        }
    } else if (flags & 0x800000) {
        func_02018e9c(object);
    }
    return 1;
}
