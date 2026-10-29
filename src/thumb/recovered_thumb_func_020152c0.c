extern int func_020151d0();
extern int func_020151f4();
extern int func_0205429c();
extern int func_02055e6c();

void func_020152c0(int param_1)

{
    int iVar1;

    if ((*(int *)(param_1 + 0x44) != 0) && (iVar1 = func_020151f4(), iVar1 == 0)) {
        func_0205429c(*(unsigned int *)(param_1 + 0x44));
    }
    if (*(int *)(param_1 + 0x48) != 0) {
        func_02055e6c();
        *(unsigned int *)(param_1 + 0x48) = 0;
    }
    if (*(int *)(param_1 + 0x4c) != 0) {
        func_020151d0(*(int *)(param_1 + 0x4c),0);
    }
    return;
}
