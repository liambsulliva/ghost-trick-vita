extern unsigned char func_0200b794();
extern int func_0200b848();

unsigned int func_0200bc10(int param_1)

{
    unsigned char bVar1;
    unsigned int uVar2;

    uVar2 = func_0200b848(param_1 + 0x14);
    if ((uVar2 != 0) && (bVar1 = func_0200b794(uVar2), bVar1)) {
        return 1;
    }
    return 0;
}
