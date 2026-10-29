extern int func_02009900();
extern int func_020153d8();
extern int func_0202f7e8();
extern int func_0205bfb8();

void func_0205e2dc(int param_1)

{
    int iVar1;
    unsigned int uVar2;

    if (*(int *)(param_1 + 0x84) == 0) {
        iVar1 = func_0205bfb8(((unsigned int)0x020cfab8u));
        *(int *)(param_1 + 0x84) = iVar1;
        if (iVar1 != 0) {
            func_020153d8(iVar1,param_1);
            uVar2 = func_02009900();
            func_0202f7e8(uVar2,*(unsigned int *)(param_1 + 0x84),7,0);
        }
    }
    return;
}
