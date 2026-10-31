extern const char data_ov005_020d4868[];
extern const char data_ov005_020d4880[];
extern const char data_ov005_020d4890[];
extern const char data_ov005_020d48a0[];
extern const char data_ov005_020d48b8[];
extern const char data_ov005_020d48c8[];
extern const char data_ov005_020d48d4[];
extern int func_02008444(void);
extern void func_02010298(int object, const char *name);
extern int func_02022dfc(int object);
extern void func_02022f34(int object, const char *name, unsigned int value);
extern void func_02022f4c(int object);
extern int func_02031b20(int object);

void func_ov005_020d40dc(int param_1)
{
    signed char state;

    state = *(signed char *)(param_1 + 0x50);
    if (state == 1) {
        func_02022f34(func_02031b20(param_1), data_ov005_020d4868, *(unsigned int *)(param_1 + 0x48));
        func_02022f34(func_02022dfc(param_1), data_ov005_020d4880, 0);
        *(unsigned char *)(param_1 + 0x51) = *(unsigned char *)(param_1 + 0x51) | 4;
        return;
    }
    if (state == 2) {
        *(unsigned char *)(param_1 + 0x51) = *(unsigned char *)(param_1 + 0x51) & ~4;
        func_02022f34(func_02031b20(param_1), data_ov005_020d4890, 0);
        return;
    }
    if (state == 4) {
        func_02022f34(func_02031b20(param_1), data_ov005_020d48a0, 0);
        func_02022f4c(func_02022dfc(param_1));
        *(unsigned char *)(param_1 + 0x51) = *(unsigned char *)(param_1 + 0x51) & ~4;
        return;
    }
    if (state == 3) {
        func_02010298(func_02008444(), data_ov005_020d48b8);
        func_02022f34(func_02031b20(param_1), data_ov005_020d48c8, 0);
        return;
    }
    if ((((unsigned int)*(unsigned char *)(param_1 + 0x51) << 0x1e) >> 0x1f) != 0) {
        func_02022f34(func_02031b20(param_1), data_ov005_020d48d4, *(unsigned int *)(param_1 + 0x48));
        func_02022f34(func_02022dfc(param_1), data_ov005_020d4880, 0);
        *(unsigned char *)(param_1 + 0x51) = *(unsigned char *)(param_1 + 0x51) | 4;
        return;
    }
    func_02022f34(func_02031b20(param_1), data_ov005_020d48c8, 0);
}
