extern char data_020a2550;
extern int func_02075d50();

unsigned int func_02074534(void)

{
    unsigned int uVar1;

    uVar1 = func_02075d50();
    return *(unsigned int *)(((int)&data_020a2550) + uVar1 * 4);
}
