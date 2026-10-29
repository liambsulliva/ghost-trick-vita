extern int func_02008444();
extern int func_02010718();
extern int func_02010b74();

void func_02045568(int param_1)

{
    unsigned int uVar1;

    if (*(char *)(param_1 + 199) == '\0') {
        uVar1 = func_02008444();
        func_02010718(uVar1,0);
        func_02010b74(param_1);
    }
    return;
}
