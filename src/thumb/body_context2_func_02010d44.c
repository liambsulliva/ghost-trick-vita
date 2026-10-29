struct FlagWord {
    unsigned int before : 8;
    unsigned int flag : 1;
};
extern int func_02046b90();

unsigned int func_02010d44(int param_1)

{
    unsigned int uVar1;

    if (*(int *)(param_1 + ((int)0x00000fb4u)) == 0) {
        return 0;
    }
    uVar1 = func_02046b90(*(int *)(param_1 + ((int)0x00000fb4u)));
    if (uVar1 == 0) {
        return 0;
    }
    if (((struct FlagWord *)(param_1 + 0xc))->flag != 0) {
        return 0;
    }
    return 1;
}
