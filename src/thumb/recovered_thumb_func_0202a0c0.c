extern int func_0202a890();

unsigned int func_0202a0c0(int param_1,int param_2)

{
    unsigned int uVar1;

    if (*(char *)(param_1 + 0x24) == '\x01') {
        return 0;
    }
    if ((param_2 == 0) || (param_2 == 0x21)) {
        return 0;
    }
    uVar1 = func_0202a890(param_1,param_2,0x100,0,0);
    return uVar1;
}
