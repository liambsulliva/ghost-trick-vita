void func_02030470(int param_1)
{
    volatile unsigned short *puVar1;

    puVar1 = (volatile unsigned short *)0x04000060u;
    if (param_1 != 0) {
        *puVar1 = (unsigned short)((*puVar1 & 0xffffcfffu) | 0x20u);
        return;
    }
    *puVar1 = (unsigned short)(0x0000cfdfu & *puVar1);
}
