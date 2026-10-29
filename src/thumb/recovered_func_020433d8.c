extern int func_02009900();
extern int func_020433f8();

unsigned int func_020433d8(void)

{
    int iVar1;

    func_02009900();
    iVar1 = func_020433f8();
    if ((unsigned int)(((int)0x0000010fu) - iVar1) < 0x11) {
        return 0;
    }
    return 1;
}
