extern unsigned int func_020266dc();
extern void func_02058a80();
extern void func_020083b0();
extern void func_0206167c();
extern void func_02060d54();
extern void func_02060f34();

void func_020610ac(unsigned char *param_1, unsigned int param_2, unsigned int param_3)
{
    unsigned int uVar1;
    unsigned int uVar2;
    int local[3];
    int one;
    int two;

    uVar1 = func_020266dc(param_2, param_3);
    if (uVar1 != 0) {
        if (*(unsigned int *)(param_1 + 0xfc) == uVar1) {
            int mask;
            mask = 8;
            param_1[0x7d] = param_1[0x7d] & ~mask;
            return;
        }
        uVar2 = param_1[0x7d] | 8;
        param_1[0x7d] = (unsigned char)uVar2;
        func_02058a80((unsigned char *)local, uVar1);
        func_020083b0(param_1 + 0x98, local);
        func_0206167c(param_1);
        func_02060d54(param_1, 1);
        one = 1;
        two = 2;
        param_1[0x7c] = param_1[0x7c] & ~one;
        param_1[0x7c] = param_1[0x7c] & ~two;
        param_1[0x80] = 0;
        func_02060f34(param_1);
    }
    return;
}
