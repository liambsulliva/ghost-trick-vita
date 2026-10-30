void func_02030410(int param_1, unsigned short param_2)
{
    volatile unsigned short *puVar1;

    if (param_1 != 0) {
        puVar1 = (volatile unsigned short *)0x04000060;
        *puVar1 = (*puVar1 & 0xffffcfffu) | 4;
        *(volatile unsigned short *)0x04000340 = param_2;
        return;
    }
    puVar1 = (volatile unsigned short *)0x04000060;
    *puVar1 = 0xcffbu & *puVar1;
}
