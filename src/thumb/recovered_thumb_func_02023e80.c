extern int func_02023e28();
extern int func_02023eac();

unsigned int func_02023e80(int param_1,unsigned int param_2)

{
    unsigned int uVar1;
    unsigned int uVar2;

    uVar2 = *(unsigned int *)(param_1 + (param_2 & 0x3ff) * 4 + 0x20);
    uVar1 = func_02023e28(uVar2);
    if ((((unsigned int)0xfffffc00u) & param_2) == uVar1) {
        uVar2 = func_02023eac(uVar2);
        return uVar2;
    }
    return 0;
}
