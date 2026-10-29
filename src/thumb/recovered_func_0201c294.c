extern int func_0207b8a0();

void func_0201c294(int param_1,int param_2)

{
    unsigned int uVar1;

    *(int *)(param_1 + 0x2c) = param_2;
    if (param_2 == 0) {
        uVar1 = 0;
        *(unsigned int *)(param_1 + 0x34) = 0;
    }
    else {
        *(unsigned int *)(param_1 + 0x34) = 0x10000;
        uVar1 = func_0207b8a0();
    }
    *(unsigned int *)(param_1 + 0x30) = uVar1;
    return;
}
