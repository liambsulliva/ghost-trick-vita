extern int func_0203c6a8();
extern int func_0201572c();

void func_0203d4b4(unsigned char *param_1)
{
    int iVar1;

    iVar1 = func_0203c6a8(param_1, 0, 0, 0);
    if (((iVar1 == 0) && ((unsigned char)(param_1[4] + 255) <= 2)) && (iVar1 = func_0201572c(), iVar1 == 0)) {
        if (param_1[4] == 3) {
            param_1[0x50] = 1;
            param_1[0x55] = 1;
        }
        else {
            param_1[0x55] = 0;
        }
        param_1[4] = 12;
        *(unsigned short *)(param_1 + 0x4a) = 0;
    }
    return;
}
