extern int func_02075f84();
extern int func_020779c8();
extern int func_02077a3c();
extern int func_02077ad0();
extern int func_02077bc0();

unsigned char *func_0202f01c(unsigned char *param_1)

{
    unsigned short buf[6];
    unsigned short uVar1;

    param_1[3] = 0;
    func_020779c8();
    if (func_02077a3c(buf) == 0) {
        func_02075f84();
    }
    else {
        func_02077ad0(buf);
    }
    *(unsigned short *)(param_1 + 0x16) = 0;
    uVar1 = *(unsigned short *)(param_1 + 0x16);
    *param_1 = 0;
    param_1[1] = 0;
    *(unsigned short *)(param_1 + 0x14) = uVar1;
    *(unsigned short *)(param_1 + 0x12) = uVar1;
    *(unsigned short *)(param_1 + 0x10) = uVar1;
    *(unsigned short *)(param_1 + 6) = uVar1;
    *(unsigned short *)(param_1 + 4) = uVar1;
    *(unsigned short *)(param_1 + 8) = 0;
    param_1[2] = 0;
    func_02077bc0();
    return param_1;
}
