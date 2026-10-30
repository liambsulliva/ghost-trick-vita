extern int func_02076240();
extern int WaitByLoop();

void func_0206b1b8(int param_1, int param_2)
{
    if (func_02076240(14, param_1, 0) == 0) {
        return;
    }
    do {
        WaitByLoop(param_2);
    } while (func_02076240(14, param_1, 0) != 0);
}
