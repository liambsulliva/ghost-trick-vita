extern int func_0202fa2c();

void func_0202f7e8(int param_1,int param_2,unsigned int param_3,int param_4)

{
    unsigned char uVar1;

    if ((param_2 != 0) && (param_3 < 0xf)) {
        func_0202fa2c();
        uVar1 = 5;
        if (param_4 == 0) {
            uVar1 = 2;
        }
        *(unsigned char *)(param_2 + 0xc) = uVar1;
        *(char *)(param_2 + 0xd) = (char)param_3;
        *(int *)(param_1 + 0x124) = *(int *)(param_1 + 0x124) + 1;
    }
    return;
}
