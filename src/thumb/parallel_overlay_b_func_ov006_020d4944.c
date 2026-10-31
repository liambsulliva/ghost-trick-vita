extern void func_ov006_020d4968(unsigned char *, int, int);

void func_ov006_020d4944(unsigned char *param_1, int param_2, int param_3, int param_4)
{
    param_1[0x160] = param_2;
    *(unsigned short *)(param_1 + 0x15c) = 0;
    param_1[0x161] = 0;
    *(unsigned short *)(param_1 + 0x15e) = 0;
    func_ov006_020d4968(param_1, param_3, param_4);
}
