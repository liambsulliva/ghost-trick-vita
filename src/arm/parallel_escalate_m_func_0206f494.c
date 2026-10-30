int func_0206f494(int param_1)
{
    int rc;

    rc = *(volatile unsigned short *)0x04000004 & 8;
    if (param_1 != 0) {
        *(volatile unsigned short *)0x04000004 = *(volatile unsigned short *)0x04000004 | 8;
    }
    else {
        *(volatile unsigned short *)0x04000004 = *(volatile unsigned short *)0x04000004 & 0xfff7;
    }
    return rc;
}
