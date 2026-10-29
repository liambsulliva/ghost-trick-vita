extern char data_020d0f0c;
extern int func_02073af0();
extern int func_02073cc4();
extern int func_02075c98();
extern int func_02075cac();

void func_020740d0(int param_1)

{
    unsigned int uVar1;
    int iVar2;

    uVar1 = func_02075c98();
    iVar2 = **(int **)(((int)&data_020d0f0c) + 8);
    if (param_1 != 0) {
        *(int *)(iVar2 + 0x78) = param_1;
        func_02073af0(param_1,iVar2);
    }
    *(unsigned int *)(iVar2 + 100) = 0;
    func_02073cc4();
    func_02075cac(uVar1);
    return;
}
