extern int func_02008444();
extern int func_02010384();
extern int func_02010b74();
extern int func_02027458();

void func_020459a8(int param_1)

{
    unsigned int uVar1;

    if (*(char *)(param_1 + 199) == '\0') {
        uVar1 = func_02008444();
        func_02010384(uVar1,0);
        func_02027458(*(unsigned int *)(param_1 + 0x130));
        func_02010b74(param_1);
    }
    return;
}
