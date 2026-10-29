extern int func_0200bc7c();
extern int func_0200bcd0();

void func_0200c61c(unsigned char *param_1)

{
    int iVar1;

    if (*(int *)(param_1 + ((int)0x00001528u)) == 1) {
        func_0200bcd0((unsigned int)param_1);
        func_0200bc7c(param_1);
        iVar1 = ((int)0x00000a88u);
        func_0200bcd0((unsigned int)(param_1 + ((int)0x00000a88u)));
        func_0200bc7c(param_1 + iVar1);
    }
    return;
}
