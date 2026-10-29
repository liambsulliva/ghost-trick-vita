extern int func_020316fc();

unsigned int func_020316dc(void)

{
    unsigned int uVar1;

    uVar1 = func_020316fc();
    return uVar1 & ~((uVar1 & 0x40) << 1) & ~((int)(uVar1 & 0x20) >> 1) & 0xffff;
}
