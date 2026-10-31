struct State
{
    unsigned int before[4];
    int data;
};

extern struct State data_020b18a0;
extern void func_0207c594();
extern void func_02003138();
extern int func_02003298();
extern int *func_02000d9c();

int func_020034cc(unsigned int param_1, int param_2, int param_3)
{
    int *piVar1;
    int iVar2;
    int iVar3;
    int i;
    int base;
    int result;
    unsigned short buf[18];

    func_0207c594(buf, 0x24);
    iVar2 = (int)&data_020b18a0;
    iVar3 = *(int *)(*(int *)(iVar2 + 0x10) + 0x1580);
    piVar1 = func_02000d9c(iVar3, param_1, *(int *)(*(int *)(iVar3 + 0x98) + 8));
    if (piVar1 == 0) {
        return 0;
    }
    iVar3 = *(int *)(*(int *)(*(int *)(*(int *)(iVar2 + 0x10) + 0x1580) + 0x90) + *piVar1 * 0x10 + 0x14);
    if (iVar3 == 0) {
        return 0;
    }
    base = iVar3 + *(int *)(iVar3 + 0x18);
    func_02003138(base, 0, buf);
    for (i = 0; i < 0x10; i++) {
        if (buf[0] & (1 << i)) {
            result = func_02003298(base + buf[i + 2], base, param_2, param_3);
            if (result == 0) {
                break;
            }
        }
    }
    return result;
}
