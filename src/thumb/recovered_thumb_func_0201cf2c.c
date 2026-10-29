extern int func_0201cf74();
extern int func_0201cf78();

void func_0201cf2c(int param_1,int param_2)

{
    int iVar1;

    if (param_2 != 0) {
        *(unsigned char *)(param_1 + 0x36) = 1;
        *(int *)(param_1 + 0x38) = param_2;
        iVar1 = func_0201cf74(param_2);
        if (iVar1 == 2) {
            func_0201cf78(param_1 + 0x44,2);
            func_0201cf78(param_1 + 100,2);
            return;
        }
        func_0201cf78(param_1 + 0x44,1);
        func_0201cf78(param_1 + 100,1);
    }
    return;
}
