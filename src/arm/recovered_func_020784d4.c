extern int func_020783c8();

unsigned int func_020784d4(int param_1,int param_2,unsigned int param_3,unsigned int param_4)

{
    unsigned int uVar1;
    int iVar2;

    iVar2 = 0;
    if (param_1 == 0) {
        if (param_2 == 1) {
            iVar2 = 6;
        }
        if (param_2 == 0) {
            iVar2 = 7;
        }
    }
    else if (param_1 == 1) {
        if (param_2 == 1) {
            iVar2 = 4;
        }
        if (param_2 == 0) {
            iVar2 = 5;
        }
    }
    else if (param_1 == 2) {
        if (param_2 == 1) {
            iVar2 = 8;
        }
        if (param_2 == 0) {
            iVar2 = 9;
        }
    }
    if (iVar2 == 0) {
        return ((unsigned int)0x0000ffffu);
    }
    uVar1 = func_020783c8(iVar2,0,0,param_3,param_4);
    return uVar1;
}
