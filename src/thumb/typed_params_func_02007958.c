void func_02007958(unsigned int param_1)

{
    unsigned int uVar1;

    if ((int)param_1 < 0) {
        uVar1 = 0;
    }
    else {
        uVar1 = param_1 & 0x7f;
    }


    func_02076714(uVar1);
    return;
}
