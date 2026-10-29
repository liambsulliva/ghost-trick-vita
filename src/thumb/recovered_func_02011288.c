extern int func_020112a8();
extern int func_0201135c();

void func_02011288(int param_1)

{
    int iVar1;

    iVar1 = func_020112a8();
    if (iVar1 == 0) {
        *(unsigned char *)(param_1 + ((unsigned int)0x0000103cu)) = 1;
        func_0201135c(param_1);
    }
    return;
}
