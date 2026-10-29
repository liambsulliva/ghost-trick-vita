extern int func_020130c8();

unsigned int func_0204d2a8(int param_1)

{


    unsigned int uVar1;

    func_020130c8();
    uVar1 = (unsigned int)*(unsigned short *)(param_1 + 0x88) - ((int)0x0000800cu);
    *(short *)(param_1 + 0xd4) = (short)uVar1;
    return uVar1;
}
