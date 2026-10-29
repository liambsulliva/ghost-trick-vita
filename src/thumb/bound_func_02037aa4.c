extern char data_020cdd0c;
extern int func_02037ac4();
extern int func_020380e8();

unsigned int func_02037aa4(unsigned int param_1)

{
    int iVar1;
    unsigned int uVar2;

    iVar1 = ((int)&data_020cdd0c);
    if (*(int *)(((int)&data_020cdd0c) + 8) == 0) {
        return 0;
    }
    func_02037ac4(*(int *)(((int)&data_020cdd0c) + 8),param_1);
    uVar2 = func_020380e8(*(unsigned int *)(iVar1 + 8));
    return uVar2;
}
