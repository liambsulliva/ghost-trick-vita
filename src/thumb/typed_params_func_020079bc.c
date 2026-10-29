extern int func_020079dc();
extern int func_02076458();

void func_020079bc(unsigned int param_1,unsigned int param_2)

{
    unsigned int uVar1;

    if ((int)param_2 < 0) {
        uVar1 = 0;
    }
    else {
        uVar1 = param_2 & 0x7f;
    }
    uVar1 = func_020079dc(uVar1);
    func_02076458(param_1,uVar1);
    return;
}
