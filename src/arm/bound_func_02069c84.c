extern char data_020d02a0;
extern int func_020740d0();
extern int func_02075c98();
extern int func_02075cac();
extern int func_02075f84();

void func_02069c84(int param_1,int param_2)

{
    int iVar1;
    unsigned int uVar2;

    iVar1 = ((int)&data_020d02a0);
    uVar2 = func_02075c98();
    if (*(int *)(iVar1 + 0x14) == param_1) {
        if (*(int *)(iVar1 + 0x24) != param_2) {
            func_02075f84();
        }
    }
    else {
        while (*(int *)(iVar1 + 0x14) != -3) {
            func_020740d0(iVar1 + 0x1c);
        }
        *(int *)(iVar1 + 0x14) = param_1;
        *(int *)(iVar1 + 0x24) = param_2;
    }
    *(int *)(iVar1 + 0x18) = *(int *)(iVar1 + 0x18) + 1;
    func_02075cac(uVar2);
    return;
}
