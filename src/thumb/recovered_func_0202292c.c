extern int func_0202294c();

unsigned int func_0202292c(void)

{
    unsigned int uVar1;

    uVar1 = func_0202294c();
    return uVar1 & ~((uVar1 & 0x40) << 1) & ~((int)(uVar1 & 0x20) >> 1) & 0xffff;
}
