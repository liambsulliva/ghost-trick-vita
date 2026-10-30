extern unsigned char data_020c9e88[];
extern unsigned char data_020c9ec8[];
extern int func_ov001_020d3a64();

void func_ov001_020d3a40(int param_1)
{
    unsigned int uVar1;

    uVar1 = 0xf;
    uVar1 = ~uVar1;
    *(short *)(data_020c9e88 + 0x12) = (short)uVar1;
    *(short *)(data_020c9ec8 + 0x26) = (short)uVar1;
    func_ov001_020d3a64(param_1, 0, 0, 0, 0);
}
