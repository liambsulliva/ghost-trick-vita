extern void func_ov006_020d4968(unsigned char *, int, int);

void func_ov006_020d4608(unsigned char *param_1, int param_2, int param_3, int param_4)
{
    *(int *)(param_1 + 0x158) = param_2;
    param_1[0x160] = 0;
    *(unsigned short *)(param_1 + 0x15c) = 0;
    param_1[0x161] = 0;
    param_1[0x164] = param_3;
    *(unsigned short *)(param_1 + 0x15e) = 0;
    *(int *)(param_1 + 0x168) = param_4;
    func_ov006_020d4968(param_1, 0, 1);
}
