extern int data_020cf5f8;
extern int func_02055b4c();
extern int func_020560f4();
extern int func_0203bfb4();
extern int func_020153d8();
extern void *func_02009900();
extern int func_0202f7e8();

void func_020560a4(unsigned char *param_1)
{
    int iVar1;
    void *puVar2;

    iVar1 = func_02055b4c(&data_020cf5f8);
    *(int *)(param_1 + 0x2c) = iVar1;
    if (iVar1 != 0) {
        func_020560f4(iVar1, 0x2a0, 0x2a5, 0x2aa, 0xc2, 0);
        func_0203bfb4(*(void **)(param_1 + 0x2c), 0);
        func_020153d8(*(int *)(param_1 + 0x2c), param_1);
        puVar2 = func_02009900();
        func_0202f7e8(puVar2, *(int *)(param_1 + 0x2c), 7, 0);
        return;
    }
    *(int *)(param_1 + 0x28) = 1;
}
