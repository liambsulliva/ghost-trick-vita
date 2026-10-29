extern int func_0202db3c();

void func_0202e258(int param_1)

{
    unsigned int uVar1;

    if (*(int *)(param_1 + 500) == 0) {
        for (uVar1 = 0; uVar1 < *(unsigned int *)(param_1 + 0x144); uVar1 = uVar1 + 1) {
            func_0202db3c(*(int *)(param_1 + 0x1f0) + uVar1 * 0x6c);
        }
    }
    return;
}
