extern char data_020b18a0;
extern unsigned int func_020030a8(unsigned int param_1);
extern unsigned int func_02003030(unsigned int param_1);
extern unsigned int func_0200306c(unsigned int param_1);
extern unsigned int func_020030e4(unsigned int param_1, unsigned int param_2);
extern void func_02003e30(int entry, int arg, int flag);
extern void func_02003eb4(int entry, int arg);

void func_020044ec(unsigned int param_1, unsigned int param_2, unsigned int param_3, unsigned char param_4, unsigned char param_5, int param_6, unsigned char param_7)
{
    unsigned int mask;
    char *work;
    unsigned int i;
    int entry;

    mask = 0xffff;
    if (param_1 != 0) {
        mask = mask & func_020030a8(param_1);
    }
    if (param_2 != 0xffffffff) {
        mask = mask & func_02003030(param_2);
    }
    if (param_3 != 0xffffffff) {
        mask = mask & func_0200306c(param_3);
    }
    if (param_4 != 0xff || param_5 != 0xff) {
        mask = mask & func_020030e4(param_4, (unsigned int)param_5);
    }
    work = &data_020b18a0;
    for (i = 0; i < 16; i++) {
        entry = *(int *)(work + 0x10) + 0x15bc + i * 0x4c;
        if (*(int *)(entry + 4) == 1 && (((int)mask >> i) & 1) != 0 &&
            (param_7 == *(unsigned char *)(entry + 0x12) || param_7 == 0xff)) {
            if ((*(unsigned int *)(entry + 0x3c) & 4) != 0) {
                func_02003eb4(entry, param_6);
            } else {
                func_02003e30(entry, param_6, 1);
            }
        }
    }
}
