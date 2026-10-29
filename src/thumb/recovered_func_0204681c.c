extern int func_02046838();

void func_0204681c(int param_1,unsigned int param_2)

{
    unsigned int uVar1;

    uVar1 = 0;
    do {
        func_02046838(*(unsigned int *)(param_1 + uVar1 * 4 + 0x28),param_2);
        uVar1 = uVar1 + 1;
    } while (uVar1 < 2);
    return;
}
