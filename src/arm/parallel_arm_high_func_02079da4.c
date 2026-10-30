extern int WaitByLoop();
extern int func_02076240();

void func_02079da4(int param_1)
{
    int iVar1;

    iVar1 = func_02076240(13,param_1,0);
    if (iVar1 == 0) {
        return;
    }
    do {
        WaitByLoop(1);
        iVar1 = func_02076240(13,param_1,0);
    } while (iVar1 != 0);
}
