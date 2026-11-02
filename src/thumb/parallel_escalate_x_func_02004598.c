struct State {
    unsigned int before[4];
    int data;
};

extern struct State data_020b18a0;
extern unsigned int func_020030a8(unsigned int param_1);
extern unsigned int func_02003030(unsigned int param_1);
extern unsigned int func_0200306c(unsigned int param_1);
extern unsigned int func_020030e4(unsigned int param_1, unsigned int param_2);
extern void func_02006954(char *fade, unsigned int from, int to, int mode, int time);
extern unsigned int func_0207b3dc(int param_1);
extern unsigned int func_0207b424(unsigned int param_1);
extern unsigned int func_0207bb44(unsigned int param_1, unsigned int param_2);
extern unsigned int func_0207c1d8(unsigned int param_1, unsigned int param_2);
extern unsigned int func_0207bd24(unsigned int param_1, unsigned int param_2);
extern int func_0207ba84(unsigned int param_1);
extern void func_020079bc(int param_1, int param_2);

int func_02004598(unsigned int param_1, unsigned int param_2, unsigned int param_3, unsigned int param_4, unsigned char param_5, int param_6, int param_7, unsigned int param_8)
{
    unsigned int uVar6;
    int i;
    unsigned char vol;
    char *slot;
    char *fade;
    char *group;
    unsigned int f;

    uVar6 = 0xffff;
    if (param_1 != 0) {
        uVar6 = uVar6 & func_020030a8(param_1);
    }
    if (param_2 != 0xffffffff) {
        uVar6 = uVar6 & func_02003030(param_2);
    }
    if (param_3 != 0xffffffff) {
        uVar6 = uVar6 & func_0200306c(param_3);
    }
    if (param_4 != 0xff || param_5 != 0xff) {
        uVar6 = uVar6 & func_020030e4(param_4, (unsigned int)param_5);
    }
    vol = param_6;
    for (i = 0; i < 16; i++) {
        slot = (char *)(*(int *)((char *)&data_020b18a0 + 0x10) + 0x15bc + i * 0x4c);
        if ((((int)uVar6 >> i) & 1) == 0) {
            continue;
        }
        if (*(int *)(slot + 4) != 1) {
            continue;
        }
        if ((unsigned char)slot[0x12] != param_8 && param_8 != 0xff) {
            continue;
        }
        fade = (char *)(data_020b18a0.data + 0x15d4 + i * 0x4c);
        group = (char *)(data_020b18a0.data + 0x1a7c + (unsigned char)slot[0x11] * 8);
        if (fade == 0) {
            continue;
        }
        *(unsigned int *)(slot + 0x3c) &= ~8;
        *(int *)(fade + 0x14) = 3;
        if (param_7 != 0) {
            func_02006954(fade, (unsigned char)slot[0xe], param_6, 2, param_7);
            *(unsigned int *)(slot + 0x3c) |= 8;
        } else {
            slot[0x31] = vol;
            slot[0xe] = vol;
            f = func_0207b3dc(param_6);
            f = func_0207bb44(f, func_0207b424((unsigned char)group[6]));
            f = func_0207c1d8(f, 0x42fe0000);
            f = func_0207bd24(0x3f000000, f);
            func_020079bc(i, func_0207ba84(f));
        }
    }
    return 0;
}
