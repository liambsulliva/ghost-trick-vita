extern int func_02043400();

unsigned int func_02043664(int param_1)

{
    int iVar1;
    unsigned int uVar2;

    uVar2 = 0;
    iVar1 = func_02043400(param_1,0);
    if ((iVar1 != 0) && (*(unsigned short *)(iVar1 + 0xe) != 0)) {
        uVar2 = 1;
    }
    return uVar2;
}
