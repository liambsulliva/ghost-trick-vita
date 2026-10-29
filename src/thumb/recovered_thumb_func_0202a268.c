extern int func_0202a940();
extern int func_0202a97c();

void func_0202a268(int param_1)

{
    unsigned int uVar1;

    if (*(char *)(param_1 + 0x24) != '\x01') {
        uVar1 = 0;
        do {
            func_0202a97c(param_1,uVar1 & 0xffff);
            uVar1 = uVar1 + 1;
        } while ((int)uVar1 < 2);
        if (*(short *)(param_1 + 0x26) == 0) {
            func_0202a940(param_1,0,1);
        }
    }
    return;
}
