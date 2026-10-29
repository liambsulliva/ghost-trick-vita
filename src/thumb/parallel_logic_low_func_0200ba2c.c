extern signed char func_0200b114();
extern int func_0200b848();

int func_0200ba2c(int param_1)

{
    signed char cVar1;
    unsigned int uVar2;

    uVar2 = func_0200b848(param_1 + 0x14);
    if (uVar2 != 0) {
        return (int)func_0200b114(uVar2);
    }
    return 0;
}
