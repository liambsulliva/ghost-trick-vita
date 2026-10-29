void func_02007a70(unsigned int param_1,unsigned int param_2)

{
    unsigned int uVar1;

    if ((int)param_2 < 0) {
        uVar1 = 0;
    }
    else {
        uVar1 = param_2 & 0x7f;
    }


    func_020766a8(param_1,uVar1);
    return;
}
