unsigned short func_ov004_020d6200(unsigned short param_1)

{
    unsigned short uVar1;

    uVar1 = *((unsigned short *)0x04000208u);
    *((unsigned short *)0x04000208u) = param_1;
    return uVar1;
}
