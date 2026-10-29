extern int func_02019bb4();

void func_02019b88(int param_1)

{
    if (*(int *)(param_1 + 0x54) != 1) {
        if (*(int *)(param_1 + 0x54) == 2) {
            *(char *)(param_1 + 0x4e) = *(char *)(param_1 + 0x4e) + '\x01';
            return;
        }
        func_02019bb4(param_1);
        *(char *)(param_1 + 0x3b) = *(char *)(param_1 + 0x3b) + '\x01';
    }
    return;
}
