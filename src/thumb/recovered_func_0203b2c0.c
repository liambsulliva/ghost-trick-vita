void func_0203b2c0(int param_1,unsigned int param_2,unsigned int param_3)

{
    if (param_2 < 0x100) {
        *(short *)(param_1 + 10) = (short)param_2;
    }
    if (param_3 < 0xc0) {
        *(short *)(param_1 + 0xc) = (short)param_3;
    }
    return;
}
