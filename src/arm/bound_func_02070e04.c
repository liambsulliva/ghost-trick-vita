extern char data_020d0e48;
extern int func_02071c3c();

void func_02070e04(void)

{
    unsigned int uVar1;

    uVar1 = func_02071c3c();
    *(unsigned int *)(((int)&data_020d0e48) + 0x18) = uVar1;
    return;
}
