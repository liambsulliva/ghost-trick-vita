extern int func_02008444();
extern int func_0200da04();
extern int func_02010384();
extern int func_02010b74();

void func_02045970(int param_1)

{
    unsigned int uVar1;

    if (*(char *)(param_1 + 199) == '\0') {
        uVar1 = func_02008444();
        func_02010384(uVar1,0);
        func_0200da04(*(unsigned int *)(param_1 + 0x130),*(unsigned int *)(param_1 + 0x134),
                                  *(unsigned int *)(param_1 + 0x138),((unsigned int)0x0209b63cu));
        func_02010b74(param_1);
    }
    return;
}
