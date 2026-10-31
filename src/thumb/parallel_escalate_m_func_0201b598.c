extern int func_0201b63c();
extern int func_0201bb68();

int func_0201b598(char *param_1, int param_2)
{
    int iVar1;

    *(short *)(param_1 + 0x8a) = *(short *)(param_1 + 0x34);
    iVar1 = (unsigned int)*(unsigned char *)(param_1 + 0x51) +
            (unsigned int)*(unsigned char *)(param_1 + 0x3a) *
            ((unsigned int)*(unsigned char *)(param_1 + 0x51) + (int)*(short *)(param_1 + 0x40));
    func_0201b63c(param_1 + 0x8e + (unsigned int)*(unsigned short *)(param_1 + 0x84) * 0xe,
                  (unsigned int)*(unsigned short *)(param_1 + 0x8a),
                  iVar1 & 0xffff);
    iVar1 = (unsigned int)*(unsigned char *)(param_1 + 0x51) +
            (unsigned int)*(unsigned char *)(param_1 + 0x3a) *
            ((unsigned int)*(unsigned char *)(param_1 + 0x51) + (int)*(short *)(param_1 + 0x40)) + 3;
    func_0201bb68(param_1, (unsigned int)*(unsigned short *)(param_1 + 0x88),
                  (unsigned int)*(unsigned short *)(param_1 + 0x8a), iVar1 & 0xffff);
    *(unsigned short *)(param_1 + 0x2a) = *(unsigned short *)(param_1 + 0x8c);
    *(unsigned short *)(param_1 + 0x84) = *(unsigned short *)(param_1 + 0x84) + 1;
    *(char *)(param_1 + 0x87) = 0;
    if (param_2 != 0) {
        *(unsigned short *)(param_1 + 0x80) = *(unsigned short *)(param_1 + 0x80) + 1;
    }
    return 0;
}
