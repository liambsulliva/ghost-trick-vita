extern char data_020a46c8;
extern char data_020b1280;
extern int func_020082d8();
extern int func_0200ac28();
extern int func_02052618();

unsigned int * func_02065ce0(unsigned int *param_1)

{
    unsigned int uVar1;

    func_02052618();
    uVar1 = ((unsigned int)&data_020a46c8);
    *param_1 = ((unsigned int)&data_020b1280);
    func_020082d8(param_1 + 0x2d,uVar1);
    *(unsigned short *)(param_1 + 0x30) = 0;
    param_1[0x31] = 0;
    param_1[0x32] = 0;
    *(unsigned char *)(param_1 + 0x33) = 0;
    *(unsigned short *)((int)param_1 + 0xce) = 0;
    param_1[0x34] = 0;
    *(unsigned char *)(param_1 + 0x35) = *(unsigned char *)(param_1 + 0x35) & ~0x1 | 1;
    *(unsigned char *)(param_1 + 0x35) = *(unsigned char *)(param_1 + 0x35) & ~0x2;
    *(unsigned char *)(param_1 + 0x35) = *(unsigned char *)(param_1 + 0x35) & ~0x4;
    *(unsigned char *)(param_1 + 0x35) = *(unsigned char *)(param_1 + 0x35) | 8;
    *(unsigned char *)(param_1 + 0x35) = *(unsigned char *)(param_1 + 0x35) & ~0x10;
    *(unsigned char *)(param_1 + 0x35) = *(unsigned char *)(param_1 + 0x35) & ~0x20;
    *(unsigned char *)(param_1 + 0x35) = *(unsigned char *)(param_1 + 0x35) & ~0x40;
    param_1[0x32] = 0;
    func_0200ac28(param_1);
    return param_1;
}
