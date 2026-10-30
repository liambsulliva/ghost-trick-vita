extern unsigned int func_02075c98(void);
extern void func_02075cac(unsigned int bits);
extern void func_020740d0(int param_1);

int func_02069f74(unsigned int *param_1, int param_2, unsigned int param_3, unsigned int param_4)
{
    unsigned int uVar1;
    unsigned int saved;
    unsigned char *base;

    saved = func_02075c98();
    uVar1 = param_1[1];
    if ((uVar1 & 4) != 0) {
        base = (unsigned char *)param_1 + 0xf4;
        do {
            func_020740d0((int)(base + 0x400));
            uVar1 = param_1[1];
        } while ((uVar1 & 4) != 0);
    }
    if (param_2 != 0) {
        param_1[0x13b] = param_3;
        param_1[1] = param_1[1] | 4;
        param_1[0x13c] = param_4;
    }
    func_02075cac(saved);
    return *(int *)*param_1 == 0;
}
