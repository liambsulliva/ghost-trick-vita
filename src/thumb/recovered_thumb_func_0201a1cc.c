extern int func_0201a1b0();

unsigned int func_0201a1cc(int param_1)

{
    unsigned short uVar1;

    if (*(unsigned char *)(param_1 + 0x3a) < 3) {
        uVar1 = func_0201a1b0(((unsigned int)0x020b9df4u));
        *(unsigned short *)(param_1 + (unsigned int)*(unsigned char *)(param_1 + 0x3a) * 8 + 0xe4) = uVar1;
    }
    return 1;
}
